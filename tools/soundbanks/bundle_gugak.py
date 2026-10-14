"""Copy verified Gugak downloads into an existing private sideload bundle.

Retains complete Dropbox transport archives under Original downloads and exposes
the unchanged publisher distribution ZIPs under SoundBanks. No audio is decoded
or extracted from distribution ZIPs. Existing different files are never replaced.
"""
import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import shutil
import tempfile
import zipfile


def digest(path):
    h = hashlib.sha256()
    with path.open('rb') as source:
        while block := source.read(1024 * 1024):
            h.update(block)
    return h.hexdigest()


def preserve(source, target, expected=None):
    target.parent.mkdir(parents=True, exist_ok=True)
    if target.is_symlink():
        raise ValueError('Refusing destination symlink: ' + str(target))
    if target.exists():
        checksum = hashlib.sha256()
        while block := source.read(1024 * 1024):
            checksum.update(block)
        checksum = checksum.hexdigest()
        if expected and checksum != expected:
            raise ValueError('Source checksum changed: ' + str(target))
        if digest(target) != checksum:
            raise ValueError('Refusing to replace existing different file: ' + str(target))
        return dict(bytes=target.stat().st_size, sha256=checksum)
    with tempfile.NamedTemporaryFile(dir=target.parent, delete=False) as output:
        temporary = Path(output.name)
        shutil.copyfileobj(source, output, 1024 * 1024)
    try:
        checksum = digest(temporary)
        if expected and checksum != expected:
            raise ValueError('Source checksum changed: ' + str(target))
        if target.exists():
            if digest(target) != checksum:
                raise ValueError('Refusing to replace existing different file: ' + str(target))
        else:
            # Exclusive creation also protects against a concurrent writer.
            with target.open('xb') as output, temporary.open('rb') as source:
                shutil.copyfileobj(source, output, 1024 * 1024)
            if digest(target) != checksum:
                raise ValueError('Copied file verification failed: ' + str(target))
        return dict(bytes=target.stat().st_size, sha256=checksum)
    finally:
        temporary.unlink(missing_ok=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('data_root', type=Path)
    parser.add_argument('bundle_root', type=Path)
    args = parser.parse_args()
    data, bundle = args.data_root.resolve(), args.bundle_root.resolve()
    acquisition = json.loads((data / 'Korean_Gugak/acquisition.json').read_text())
    if len(acquisition['artifacts']) != 23 or any(a['status'] != 'acquired_archive_verified' for a in acquisition['artifacts']):
        raise ValueError('All 23 publisher folders must be acquired and verified first')
    records = []
    for item in acquisition['artifacts']:
        source = data / item['relative_path']
        folder = Path(item['family']) / item['id']
        target = bundle / 'Original downloads/Korean_Gugak' / folder / source.name
        with source.open('rb') as stream:
            record = preserve(stream, target, item['sha256'])
        record.update(path=target.relative_to(bundle).as_posix(), role='original_transport_archive', download_url=item['download_url'])
        records.append(record)
        with zipfile.ZipFile(source) as archive:
            for entry in archive.infolist():
                if entry.is_dir():
                    continue
                name = PurePosixPath(entry.filename)
                if name.is_absolute() or '..' in name.parts or '\\' in entry.filename:
                    raise ValueError('Unsafe transport member: ' + entry.filename)
                if name.suffix.lower() not in ('.zip', '.pdf', '.txt', '.md'):
                    raise ValueError('Unexpected transport member: ' + entry.filename)
                if entry.file_size > 2 * 1024**3:
                    raise ValueError('Oversized transport member: ' + entry.filename)
                target = bundle / 'SoundBanks/Korean_Gugak' / folder / str(name)
                with archive.open(entry) as stream:
                    record = preserve(stream, target)
                record.update(path=target.relative_to(bundle).as_posix(), role='original_publisher_file', transport_path=item['relative_path'], original_member=entry.filename)
                if name.suffix.lower() == '.zip':
                    with zipfile.ZipFile(target) as original:
                        if original.testzip():
                            raise ValueError('Original distribution CRC failure: ' + str(target))
                        record['exs_definitions'] = sum(i.filename.lower().endswith('.exs') for i in original.infolist())
                records.append(record)
        print(item['id'], 'copied and SHA-256 verified', flush=True)
    result = dict(schema_version=1, collection='Korean_Gugak', bank_type='USER', source=acquisition['source'], license=acquisition['license'], redistribution=acquisition['redistribution'], files=records,
                  note='Complete transport archives plus unchanged original share files. Older versions retained. No converted or extracted audio. Duplicate transport/member storage is intentional preservation, not required playback storage.')
    target = bundle / 'SoundBanks/Korean_Gugak/acquisition-manifest.json'
    content = (json.dumps(result, indent=2, ensure_ascii=False) + '\n').encode()
    import io
    preserve(io.BytesIO(content), target)
    print('Verified files:', len(records), 'bytes:', sum(r['bytes'] for r in records))


if __name__ == '__main__':
    main()
