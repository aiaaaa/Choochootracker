"""Expose original latest Gugak EXS/WAV files from publisher Mac disk images.

Optional macOS acquisition helper, not an application dependency. Mounts images
read-only without Finder; never runs publisher installers. Preserves the entire
Data tree under the instrument's v1.3.1 directory, unchanged, including presets
for other apps. Original ZIPs and transport archives remain intact.
"""
import argparse
import io
import json
from pathlib import Path
import subprocess
import tempfile
import zipfile

from bundle_gugak import preserve


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('bundle_root', type=Path)
    args = parser.parse_args()
    base = args.bundle_root.resolve() / 'SoundBanks/Korean_Gugak'
    records = []
    installer_links = []
    archives = sorted(base.glob('*/*/*v.1.3.1.zip'))
    if len(archives) != 23:
        raise ValueError('Expected all 23 latest original publisher ZIPs')
    for source in archives:
        with tempfile.TemporaryDirectory(prefix='choochoo-gugak-') as temporary:
            temporary = Path(temporary)
            image, mount = temporary / 'source.dmg', temporary / 'mount'
            mount.mkdir()
            with zipfile.ZipFile(source) as archive:
                images = [e for e in archive.infolist() if e.filename.endswith('.dmg') and not e.filename.startswith('__MACOSX/')]
                if len(images) != 1:
                    raise ValueError('Expected one original disk image: ' + str(source))
                with archive.open(images[0]) as stream:
                    preserve(stream, image)
            subprocess.run(['hdiutil', 'attach', str(image), '-readonly', '-nobrowse', '-mountpoint', str(mount)], check=True, stdout=subprocess.DEVNULL)
            try:
                tree = mount / 'Data'
                if not tree.is_dir():
                    raise ValueError('Publisher image has no Data tree: ' + str(source))
                for member in sorted(tree.rglob('*')):
                    if member.is_symlink():
                        installer_links.append(dict(source_zip=source.relative_to(base).as_posix(), original_path=member.relative_to(mount).as_posix(), target=str(member.readlink())))
                        continue  # Installer shortcuts point into the publisher's Logic folders.
                    if not member.is_file():
                        continue
                    relative = member.relative_to(mount)
                    target = source.parent / 'v1.3.1' / relative
                    with member.open('rb') as stream:
                        record = preserve(stream, target)
                    record.update(path=target.relative_to(base).as_posix(), source_zip=source.relative_to(base).as_posix(), disk_image_member=images[0].filename, original_path=relative.as_posix())
                    records.append(record)
            finally:
                subprocess.run(['hdiutil', 'detach', str(mount)], check=True, stdout=subprocess.DEVNULL)
        print(source.parent.name, 'original Data tree copied and verified', flush=True)
    result = dict(schema_version=1, files=records, installer_links_not_followed=installer_links, note='Byte-identical original regular files copied from read-only publisher disk images. Installer symlinks are recorded but not followed/copied. No installer execution, resampling, mapping edits or conversion. Original images remain in original ZIPs.')
    preserve(io.BytesIO((json.dumps(result, indent=2, ensure_ascii=False)+'\n').encode()), base / 'disk-image-files.json')
    print('Verified files:', len(records), 'bytes:', sum(r['bytes'] for r in records), 'EXS:', sum(r['path'].lower().endswith('.exs') for r in records))


if __name__ == '__main__':
    main()
