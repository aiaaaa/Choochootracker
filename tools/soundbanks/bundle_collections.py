"""Preserve acquired reference collections in a private sideload bundle.

Copies verified original ZIPs into instruments/USER/soundfont, other transport archives into
documents/Original downloads, and the already-extracted unchanged FluidR3Mono/Salamander
source trees. Never modifies or replaces different existing preset files.
"""
import argparse
import io
import json
from pathlib import Path

from bundle_gugak import preserve

REPO = Path(__file__).resolve().parents[2]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('data_root', type=Path)
    parser.add_argument('bundle_root', type=Path)
    args = parser.parse_args()
    data, bundle = args.data_root.resolve(), args.bundle_root.resolve()
    manifest = json.loads((REPO / 'soundbank_manifest.json').read_text())
    records = []
    package_links = []
    for collection in manifest['collections']:
        cid = collection['id']
        if cid == 'Korean_Gugak':
            continue  # Its complete transport/member/disk-image manifests are separate.
        for artifact in collection['artifacts']:
            if artifact['status'] != 'acquired_archive_verified':
                raise ValueError('Unverified artifact: ' + artifact['filename'])
            source = data / artifact['filename']
            parent = 'instruments/USER/soundfont' if source.suffix.lower() == '.zip' else 'documents/Original downloads'
            target = bundle / parent / cid / source.name
            with source.open('rb') as stream:
                record = preserve(stream, target, artifact['sha256'])
            record.update(path=target.relative_to(bundle).as_posix(), collection=cid, role='original_archive')
            records.append(record)
            print(cid, source.name, 'copied and SHA-256 verified', flush=True)
        if cid in ('FluidR3Mono', 'Salamander'):
            for source in sorted((data / cid).rglob('*')):
                if source.is_symlink():
                    package_links.append(dict(path=source.relative_to(data).as_posix(), target=str(source.readlink())))
                    continue  # Original package retains this redundant compatibility link.
                if not source.is_file():
                    continue
                target = bundle / 'instruments/USER/soundfont' / source.relative_to(data)
                with source.open('rb') as stream:
                    record = preserve(stream, target)
                record.update(path=target.relative_to(bundle).as_posix(), collection=cid, role='original_distribution_file')
                records.append(record)
    result = dict(schema_version=1, files=records, package_links_not_followed=package_links, note='Original complete collections, no audio conversion. Includes alternate distributions. Package symlinks are recorded, not followed; originals remain in source packages. Acquisition is not a claim of compatibility or redistribution approval.')
    preserve(io.BytesIO((json.dumps(result, indent=2, ensure_ascii=False)+'\n').encode()), bundle / 'instruments/USER/soundfont/reference-collection-files.json')
    print('Verified files:', len(records), 'bytes:', sum(r['bytes'] for r in records))


if __name__ == '__main__':
    main()
