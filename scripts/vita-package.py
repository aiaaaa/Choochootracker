"""Container-only packaging, no network or source mutation."""
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess

def run(*args):
    return subprocess.check_output(args, text=True).strip()

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def main(profile):
    src, out, sdk = Path('/src'), Path('/out'), Path(os.environ['VITASDK'])
    stage = out / 'package'
    # This staging directory belongs exclusively to this build/configuration.
    if stage.exists():
        shutil.rmtree(stage)
    stage.mkdir()
    shutil.copytree(src/'tracker/packaging/common', stage/'assets')
    shutil.copytree(src/'tracker/packaging/vita/sce_sys', stage/'sce_sys')
    notices = stage/'licenses'
    shutil.copytree(src/'tracker/packaging/portmaster/license', notices)
    shutil.copy(src/'LICENSE', notices/'ChooChooTracker.md')
    shutil.copytree(sdk/'arm-vita-eabi/share/licenses/SDL2', notices/'SDL2')
    shutil.copy(sdk/'share/vdpm/THIRD_PARTY_NOTICES.md', notices/'VitaSDK.md')
    shutil.copytree(sdk/'share/vdpm/licenses', notices/'VitaSDK')
    shutil.copy(src/'docs/vita.md', stage/'VITA.md')
    if profile == 'personal':
        shutil.copytree(src/'tracker/src/experimental/mod_lucky/notices', notices/'Lucky')
        shutil.copy('/deps/curl/source/COPYING', notices/'Lucky/curl.txt')
        (stage/'certs').mkdir()
        shutil.copy('/etc/ssl/certs/ca-certificates.crt', stage/'certs')
    manifest=json.loads((out/'identity.json').read_text())
    pin=json.loads((src/'scripts/vita-sdk.json').read_text())
    manifest['toolchain']={'pin':pin, 'compiler':run('arm-vita-eabi-g++','--version'),
                         'packages':sorted(p.name for p in (sdk/'var/lib/pacman/local').iterdir() if p.is_dir())}
    manifest['dependencies']={'SDL2':run('pkg-config','--modversion','sdl2')}
    if profile=='personal':
        manifest['dependencies'].update({'curl':pin['curl'], 'curl_archive_sha256':pin['curl_archive_sha256'],
                                        'curl_library_sha256':sha(Path('/deps/curl/prefix/lib/libcurl.a')),
                                        'libxmp-lite':pin['libxmp_lite'],
                                        'libxmp_archive_sha256':pin['libxmp_archive_sha256'],
                                        'libxmp_library_sha256':sha(Path('/deps/target/prefix/lib/libxmp-lite.a')),
                                        'CA_sha256':sha(stage/'certs/ca-certificates.crt')})
    symbols=run('arm-vita-eabi-nm','-C',str(out/'native/choochootracker.elf'))
    has_lucky='modLucky::' in symbols
    if has_lucky != (profile=='personal'):
        raise RuntimeError('Lucky symbol/profile mismatch')
    if profile=='ordinary':
        link=(out/'native/link.map').read_text()
        if 'libcurl.a' in link or 'libxmp-lite.a' in link:
            raise RuntimeError('Disabled build linked Lucky dependencies')
    manifest['validation']={'host_lucky_disabled':'passed', 'vita_cross_compile':'passed',
                            'lucky_enabled_host':'run separately; see validation report',
                            'hardware':'pending', 'vita_https':'pending', 'emulator':'not run'}
    manifest['elf_size']=run('arm-vita-eabi-size',str(out/'native/choochootracker.elf'))
    shutil.copy(out/'native/eboot.bin',stage/'eboot.bin')
    run('vita-mksfoex','-s','TITLE_ID='+pin['title_id'],'-s','APP_VER=01.00',
        'ChooChooTracker',str(stage/'sce_sys/param.sfo'))
    manifest['files']={str(p.relative_to(stage)):sha(p) for p in sorted(stage.rglob('*')) if p.is_file()}
    (stage/'build-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    cmd=['vita-pack-vpk','-s',str(stage/'sce_sys/param.sfo'),'-b',str(stage/'eboot.bin')]
    for p in sorted(stage.iterdir()):
        if p.name=='eboot.bin': continue
        if p.name=='sce_sys':
            for asset in p.iterdir():
                if asset.name!='param.sfo': cmd+=['-a',str(asset)+'=sce_sys/'+asset.name]
        else: cmd+=['-a',str(p)+'='+p.name]
    cmd.append(str(out/'candidate.vpk'))
    subprocess.run(cmd,check=True)
    (out/'candidate.vpk.sha256').write_text(sha(out/'candidate.vpk')+'  candidate.vpk\n')

if __name__=='__main__':
    import sys
    main(sys.argv[1])
