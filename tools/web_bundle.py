#!/usr/bin/env python3
"""Clean-build and validate the existing Emscripten target (standard library only)."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
from datetime import datetime, timezone

ROOT = Path(__file__).resolve().parents[1]
DIST = ROOT / 'web/dist'
ASSETS = ROOT / 'tracker/packaging/common'
WRAPPER_FILES = {'index.html', 'privacy.html', 'manual.html', 'favicon.png',
                 'apple-touch-icon.png', 'site.webmanifest'}
REQUIRED = WRAPPER_FILES | {
    'lucky-license.txt', 'choochootracker.html',
    'choochootracker.js', 'choochootracker.wasm', 'choochootracker.data',
}


def git(*args):
    return subprocess.check_output(['git', *args], cwd=ROOT).decode().strip()


def check(condition, message):
    if not condition:
        raise ValueError(message)


def sha256(data):
    return hashlib.sha256(data).hexdigest()


def source_state():
    return git('rev-parse', 'HEAD'), bool(git('status', '--porcelain', '--untracked-files=normal'))


def inspect_bundle(directory):
    for name in REQUIRED:
        path = directory / name
        check(path.is_file() and not path.is_symlink() and path.stat().st_size,
              f'Missing/empty/unsafe runtime file: {name}')
    check((directory / 'choochootracker.wasm').read_bytes()[:8] == b'\0asm\1\0\0\0',
          'Invalid WebAssembly header')
    for name in WRAPPER_FILES:
        check((directory / name).read_bytes() == (ROOT / 'web' / name).read_bytes(),
              f'Stale browser wrapper: {name}')
    check((directory / 'lucky-license.txt').read_bytes() ==
          (ROOT / 'tracker/src/experimental/mod_lucky/notices/LICENSE.libxmp.txt').read_bytes(),
          'Missing or stale libxmp license')
    js = (directory / 'choochootracker.js').read_text()
    for symbol in ('webQueueButton', 'webLoadProject', 'webSaveProject',
                   'webSaveBrowserState', 'webSetPageHidden', 'webRestartAudio', 'webLoadSQ80Banks', 'IDBFS'):
        check(symbol in js, f'Missing browser integration: {symbol}')
    match = re.search(r'loadPackage\((\{"files":.*?\})\);', js)
    check(match, 'Cannot find Emscripten preload metadata; inspect SDK output')
    package = json.loads(match[1])
    data = (directory / 'choochootracker.data').read_bytes()
    check(package['remote_package_size'] == len(data), 'Preload data size mismatch')
    tracked = set(git('ls-files', '--', 'tracker/packaging/common').splitlines())
    expected = {'/' + p.removeprefix('tracker/packaging/common/'): ROOT / p for p in tracked
                if not p.startswith('tracker/packaging/common/projects/')}
    makefile = (ROOT / 'tracker/Makefile.web').read_text()
    expected.update(('/projects/' + p, ASSETS / 'projects' / p) for p in
                    re.findall(r'packaging/common/projects/([^@\n]+)@', makefile))
    expected.update(('/projects/rough demos/' + p.removeprefix('tracker/packaging/web/roughdemos/'), ROOT / p)
                    for p in git('ls-files', '--', 'tracker/packaging/web/roughdemos').splitlines())
    names = [entry['filename'] for entry in package['files']]
    check(len(names) == len(set(names)) and set(names) == set(expected),
          'Preloaded assets differ from tracked packaging inputs and Makefile.web projects')
    offset = 0
    for entry in package['files']:
        relative = entry['filename'].lstrip('/')
        check('..' not in Path(relative).parts, 'Unsafe preload path')
        path = expected[entry['filename']]
        check(not path.is_symlink(), f'Symlink in runtime assets: {relative}')
        check(entry['start'] == offset and entry['end'] >= offset,
              f'Invalid preload offsets: {relative}')
        check(data[entry['start']:entry['end']] == path.read_bytes(),
              f'Stale packaged asset: {relative}')
        offset = entry['end']
    check(offset == len(data), 'Unaccounted preload data')
    return {name: sha256((directory / name).read_bytes()) for name in sorted(REQUIRED)}


def validate(directory, require_clean=False):
    info = json.loads((directory / 'build-info.json').read_text())
    commit, dirty = source_state()
    check(info['commit'] == commit, 'Bundle belongs to another Git commit')
    if os.environ.get('GITHUB_SHA'):
        check(commit == os.environ['GITHUB_SHA'], 'Checkout differs from workflow source SHA')
    if require_clean:
        check(not dirty and not info['dirty'], 'Deployment requires a clean committed source tree')
    check({p.name for p in directory.iterdir()} == REQUIRED | {'build-info.json'},
          'Unexpected files in bundle; rebuild from clean output')
    check(info['files'] == inspect_bundle(directory), 'Bundle hashes do not match build-info.json')
    check(info['server_files'] == {'api/lucky.js': sha256((ROOT / 'api/lucky.js').read_bytes())},
          'Lucky relay source differs from build-info.json')
    print(f'Validated {len(REQUIRED)} runtime files, packaged assets and commit {commit}')


def build():
    # Never package private ignored USER collections from a developer's checkout.
    extras = git('ls-files', '--others', '--', 'tracker/packaging/common', 'tracker/packaging/web')
    check(not extras, 'Untracked/private packaging files; use a clean checkout for web builds')
    commit, dirty = source_state()
    check(os.environ.get('GITHUB_ACTIONS') != 'true' or not dirty,
          'CI must compile a clean source checkout')
    compiler = os.environ.get('EMXX', 'em++')
    version = subprocess.check_output([compiler, '--version'], text=True).splitlines()[0]
    # Only generated web output is removed; native output and user assets are untouched.
    for directory in (ROOT / 'tracker/build/web', DIST):
        check(not directory.is_symlink(), f'Refusing to clean symlink: {directory}')
        if directory.exists():
            shutil.rmtree(directory)
    try:
        subprocess.run([
            'make', '-C', 'tracker', '-j2', '-f', 'Makefile.web', 'web-deploy',
            f'EMXX={compiler}', 'COMMON_CFLAGS=-std=c++17 -Wall -g -Os -DTEST',
        ], cwd=ROOT, check=True)
        files = inspect_bundle(DIST)
        info = {
            'commit': commit, 'dirty': dirty, 'emscripten': version,
            'built_at': datetime.now(timezone.utc).isoformat(),
            'workflow_run': os.environ.get('GITHUB_RUN_ID'), 'files': files,
            'server_files': {'api/lucky.js': sha256((ROOT / 'api/lucky.js').read_bytes())},
        }
        (DIST / 'build-info.json').write_text(json.dumps(info, indent=2) + '\n')
        validate(DIST, require_clean=os.environ.get('GITHUB_ACTIONS') == 'true')
    except BaseException:
        # Failed compilation must not leave deployable-looking output.
        if DIST.exists():
            shutil.rmtree(DIST)
        raise


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('command', choices=('build', 'validate'))
    parser.add_argument('--directory', type=Path, default=DIST)
    parser.add_argument('--require-clean', action='store_true')
    args = parser.parse_args()
    if args.command == 'build':
        build()
    else:
        validate(args.directory, args.require_clean)


if __name__ == '__main__':
    try:
        main()
    except (ValueError, OSError, subprocess.CalledProcessError, KeyError) as error:
        sys.exit(f'Web bundle failed: {error}')
