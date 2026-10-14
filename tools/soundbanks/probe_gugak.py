"""Evaluate every acquired Gugak EXS definition; retain separate cache-budget runs."""
import argparse
import json
from pathlib import Path
import subprocess
import zipfile

REPO = Path(__file__).resolve().parents[2]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('bundle_root', type=Path)
    parser.add_argument('results_root', type=Path)
    parser.add_argument('--probe', type=Path, default=REPO / 'tracker/build/soundbanks/probe')
    args = parser.parse_args()
    root = args.bundle_root.resolve() / 'SoundBanks'
    base = root / 'Korean_Gugak'
    args.results_root.mkdir(parents=True, exist_ok=True)
    packaging, legacy = [], []
    for source in sorted(base.rglob('*.zip')):
        with zipfile.ZipFile(source) as archive:
            names = [e.filename for e in archive.infolist() if not e.filename.startswith('__MACOSX/') and not Path(e.filename).name.startswith('._')]
            count = sum(n.lower().endswith('.exs') for n in names)
            packaging.append(dict(source=source.relative_to(root).as_posix(), exs_definitions=count, disk_image=any(n.lower().endswith('.dmg') for n in names)))
            if count:
                legacy.append(source)
    latest = sorted(base.glob('*/*/v1.3.1/Data/**/*.exs'))
    if len(latest) != 41:
        raise ValueError('Expected 41 original v1.3.1 EXS definitions; finish disk-image preparation first')
    (args.results_root / 'gugak-packaging.json').write_text(json.dumps(packaging, indent=2, ensure_ascii=False)+'\n')

    def relative(value):
        if isinstance(value, str):
            return value.replace(str(root)+'/', '')
        if isinstance(value, list):
            return [relative(v) for v in value]
        if isinstance(value, dict):
            return {k: relative(v) for k, v in value.items()}
        return value

    for label, sources, budget in [('legacy', legacy, 256), ('latest', latest, 256), ('latest-64', latest, 64)]:
        count = loaded = 0
        with (args.results_root / ('gugak-'+label+'.jsonl')).open('w') as dest:
            for source in sources:
                result = subprocess.run([str(args.probe), str(source), '--instruments', '--root='+str(root), '--cache-mb='+str(budget)], text=True, capture_output=True, check=True)
                for line in result.stdout.splitlines():
                    record = relative(json.loads(line))
                    dest.write(json.dumps(record, ensure_ascii=False)+'\n')
                    count += 1
                    loaded += bool(record.get('loaded'))
                dest.flush()
        print(label, loaded, '/', count, 'loaded at', budget, 'MiB', flush=True)


if __name__ == '__main__':
    main()
