"""Reacquire the exact original artifacts recorded in soundbank_manifest.json.

Does not convert, extract, or install samples. Run explicitly; never from the app.
Uses curl for resumable publisher downloads, then verifies the recorded SHA-256.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess


def sha256(path):
    h = hashlib.sha256()
    with path.open('rb') as f:
        while data := f.read(1024 * 1024):
            h.update(data)
    return h.hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('destination', type=Path)
    parser.add_argument('--manifest', type=Path, default=Path(__file__).resolve().parents[2] / 'soundbank_manifest.json')
    parser.add_argument('--collection', action='append', help='Collection ID; repeat to select several. Default: all acquired collections.')
    args = parser.parse_args()
    manifest = json.loads(args.manifest.read_text())
    args.destination.mkdir(parents=True, exist_ok=True)
    failures = []
    for collection in manifest['collections']:
        if args.collection and collection['id'] not in args.collection:
            continue
        if collection['id'] == 'Korean_Gugak':
            print('Korean_Gugak: use acquire_gugak.py explicitly for the private USER collection; its Dropbox transport ZIP changes between downloads.')
            continue
        if not collection.get('artifacts'):
            print(collection['id'], 'not acquired:', collection.get('limitation', 'No verified original download'))
        for artifact in collection.get('artifacts', []):
            name = artifact['filename']
            if Path(name).name != name or not artifact.get('sha256'):
                raise ValueError('Unverified artifact: ' + name)
            target = args.destination / name
            if target.exists():
                if sha256(target) == artifact['sha256']:
                    print('Verified:', name)
                    continue
                failures.append(name + ': existing file has a different checksum; preserved')
                continue
            partial = target.with_name(target.name + '.part')
            try:
                subprocess.run(['curl', '--fail', '--location', '--retry', '3', '--continue-at', '-', '--output', str(partial), artifact['download_url']], check=True)
                if sha256(partial) != artifact['sha256']:
                    raise ValueError('Downloaded checksum differs from acquisition record')
                partial.rename(target)
                print('Acquired and verified:', name)
            except (subprocess.CalledProcessError, ValueError) as error:
                failures.append(name + ': ' + str(error))
    if failures:
        raise SystemExit('\n'.join(failures))


if __name__ == '__main__':
    main()
