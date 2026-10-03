"""Small history-preserving Vita candidate builder. Never promotes or deploys."""
import argparse
import datetime
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import uuid
import zipfile
from vita_assets import validate_livearea

ROOT=Path(__file__).resolve().parent.parent

def run(args, cwd=ROOT, capture=False):
    return subprocess.run([str(a) for a in args],cwd=cwd,check=True,text=True,
                          stdout=subprocess.PIPE if capture else None).stdout

def git(*args, cwd=ROOT):
    return run(['git',*args],cwd,True).strip()

def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()

def clean(root):
    changes=git('status','--porcelain','--untracked-files=normal',cwd=root)
    if changes: raise RuntimeError('Commit only the intended changes in this isolated worktree first. Refusing a dirty build/update:\n'+changes)

def pin(root=ROOT):
    return json.loads((root/'scripts/vita-sdk.json').read_text())

def docker_base(root, mounts=()):
    p=pin(root)
    return ['docker','run','--rm','--platform',p['platform'],*mounts,'--entrypoint','/bin/bash',p['image']]

def check_ids(root, source=None):
    text=git('show',source+':chipnomad_lib/project_instruments.h',cwd=root) if source else (root/'chipnomad_lib/project_instruments.h').read_text()
    match=re.search(r'enum\s+class\s+ModulationType\s*:\s*uint8_t\s*\{([^}]+)',text)
    if not match: raise RuntimeError('Cannot inspect MOD source IDs; review the upstream source model before updating.')
    enum=match.group(1)
    expected={'ADSR':0,'AHD':1,'LFO':2,'SLFO':3,'FLFO':4,'StickLinear':5,'StickVelocity':6,'StickRate':7,'FrontTouch':8,'RearTouch':9}
    value=-1
    for part in re.sub(r'//[^\n]*','',enum).split(','):
        part=part.strip()
        if not part: continue
        name,*assigned=part.split('=')
        name=name.strip(); value=int(assigned[0].strip(),0) if assigned else value+1
        if name=='totalCount': continue
        if (name in expected and value!=expected[name]) or (value in expected.values() and expected.get(name)!=value):
            raise RuntimeError(f'MOD source ID collision: {name}={value}. Review serialization before merging/building.')

def sfo_values(data):
    magic,version,keys,values,count=struct.unpack_from('<5I',data)
    if magic!=0x46535000: raise ValueError('Invalid SFO')
    result={}
    for i in range(count):
        key,kind,length,maximum,offset=struct.unpack_from('<HHIII',data,20+i*16)
        name=data[keys+key:].split(b'\0',1)[0].decode()
        result[name]=data[values+offset:values+offset+length].rstrip(b'\0').decode(errors='replace')
    return result

def verify(path, expected_profile=None):
    path=Path(path).resolve()
    sidecar=Path(str(path)+'.sha256')
    if not sidecar.exists() or sidecar.read_text().split()[0]!=sha(path):
        raise RuntimeError('Missing or mismatched VPK checksum sidecar')
    with zipfile.ZipFile(path) as z:
        names=z.namelist()
        if len(names)!=len(set(names)) or any(n.startswith('/') or '..' in Path(n).parts for n in names):
            raise RuntimeError('Unsafe or duplicate package entry')
        bad=z.testzip()
        if bad: raise RuntimeError('Corrupt ZIP entry: '+bad)
        m=json.loads(z.read('build-manifest.json'))
        profile=m['profile']
        if profile not in ('ordinary','personal') or (expected_profile and expected_profile!=profile):
            raise RuntimeError('Unexpected package profile')
        if m['flags']['CHOOCHOO_EXPERIMENTAL_MOD_LUCKY'] != int(profile=='personal'):
            raise RuntimeError('Feature/profile mismatch')
        for key in ('application_commit','personal_source_commit','vita_integration_revision'):
            if not re.fullmatch('[0-9a-f]{40}',m[key]): raise RuntimeError('Missing exact identity: '+key)
        relocations=m.get('validation',{}).get('elf_relocations',{})
        if any(not isinstance(relocations.get(name),int) or relocations[name]<=0
               for name in ('.rel.text','.rel.init_array')):
            raise RuntimeError('Package lacks verified Vita code/constructor relocations; rebuild with the corrected linker settings')
        thread_symbols=['pthread_cancel','pthread_create','pthread_once'] if profile=='personal' else ['pthread_cancel']
        if m.get('validation',{}).get('pthread_symbols') != thread_symbols:
            raise RuntimeError('Package lacks verified C++ pthread activation; rebuild with -pthread')
        required={'eboot.bin','sce_sys/param.sfo','sce_sys/icon0.png','VITA.md','licenses/ChooChooTracker.md'}
        if profile=='personal': required|={'certs/ca-certificates.crt','licenses/Lucky/LICENSE.libxmp.txt'}
        elif any(n.startswith(('certs/','licenses/Lucky/')) for n in names): raise RuntimeError('Ordinary package contains Lucky-only assets')
        if not required.issubset(names): raise RuntimeError('Missing required package assets')
        for folder in ('assets/fonts/','assets/title/','assets/projects/','assets/pitch-tables/'):
            if not any(n.startswith(folder) for n in names): raise RuntimeError('Missing '+folder)
        if sfo_values(z.read('sce_sys/param.sfo'))['TITLE_ID']!=pin()['title_id']:
            raise RuntimeError('Title identity changed')
        files={n for n in names if not n.endswith('/') and n!='build-manifest.json'}
        if files!=set(m['files']): raise RuntimeError('Manifest file inventory mismatch')
        for name,digest in m['files'].items():
            if hashlib.sha256(z.read(name)).hexdigest()!=digest: raise RuntimeError('Asset checksum mismatch: '+name)
        validate_livearea(z.read)
        if z.read('eboot.bin')[:4]!=b'SCE\0': raise RuntimeError('eboot is not a Vita SELF')
    print(f'Verified {profile}: {path}\nSHA256 {sha(path)}')
    return m

def doctor(root=ROOT):
    print('Source:',git('rev-parse','HEAD',cwd=root),'branch:',git('branch','--show-current',cwd=root))
    print('Working tree:',git('status','--short',cwd=root) or 'clean')
    print('SDK:',pin(root)['image'])
    check_ids(root)
    validate_livearea(lambda name: (root/'tracker/packaging/vita'/name).read_bytes())
    if not shutil.which('docker'): raise RuntimeError('Install/start Docker with Linux containers; no desktop/PortMaster toolchain changes are needed.')
    run(['docker','info','--format','{{.ServerVersion}}'])
    try: run(['docker','image','inspect',pin(root)['image']],capture=True)
    except subprocess.CalledProcessError:
        raise RuntimeError('SDK image missing. Run: docker pull '+pin(root)['image'])
    run(docker_base(root)+['-lc','set -e; command -v arm-vita-eabi-g++; command -v vita-elf-create; command -v vita-make-fself; command -v vita-mksfoex; command -v vita-pack-vpk; arm-vita-eabi-g++ --version; test -f "$VITASDK/arm-vita-eabi/lib/libpthread.a"; test -s /etc/ssl/certs/ca-certificates.crt; PKG_CONFIG_LIBDIR="$VITASDK/arm-vita-eabi/lib/pkgconfig" pkg-config --modversion sdl2 libcurl'])
    print('doctor passed; no source, dependency or working-branch changes made')

def build(root, profile):
    clean(root); check_ids(root)
    validate_livearea(lambda name: (root/'tracker/packaging/vita'/name).read_bytes())
    run([sys.executable, root/"scripts/test_vita_workflow.py"], root)
    revision=git('rev-parse','HEAD',cwd=root)
    personal=json.loads((root/'personal-features.json').read_text())
    source=personal['ports']['vita']['source_commit']
    git('merge-base','--is-ancestor',source,revision,cwd=root)
    recipes=''.join(sha(root/'scripts'/name) for name in ('build-vita-curl.sh','build-mod-lucky-dependency.sh'))
    config=hashlib.sha256((json.dumps(pin(root),sort_keys=True)+profile+recipes).encode()).hexdigest()[:16]
    work=root/'.tmp/vita'/config/revision
    deps=root/'.tmp/vita'/config/'deps'
    work.mkdir(parents=True,exist_ok=True); deps.mkdir(parents=True,exist_ok=True)
    # Atomic mkdir prevents concurrent invocations sharing objects or dependency setup.
    lock=work.parent/'build.lock'
    try: lock.mkdir()
    except FileExistsError: raise RuntimeError(f'Build already running (or interrupted). Inspect then remove {lock} only if idle.')
    try:
        identity={'schema_version':1,'application_commit':revision,'personal_source_commit':source,
                  'upstream_base':personal.get('upstream_commit'),'vita_integration_revision':revision,
                  'profile':profile,'flags':{'CHOOCHOO_EXPERIMENTAL_MOD_LUCKY':int(profile=='personal')},
                  'command':f'scripts/vita.sh build --profile {profile}',
                  'configuration_key':config,'created_utc':datetime.datetime.now(datetime.timezone.utc).isoformat()}
        (work/'identity.json').write_text(json.dumps(identity,indent=2)+'\n')
        cmd=docker_base(root,['-v',str(root)+':/src:ro','-v',str(work)+':/out','-v',str(deps)+':/deps'])
        with (work/'build.log').open('w') as log:
            print('Building',revision,profile,'— log:',work/'build.log',flush=True)
            subprocess.run(cmd+['/src/scripts/vita-container.sh',profile],check=True,stdout=log,stderr=subprocess.STDOUT)
        verify(work/'candidate.vpk',profile)
        # Immutable candidate filename; never touches promoted/last-known-good builds.
        dest=root/'releases/vita/candidates'/f'{revision[:12]}-{profile}-{uuid.uuid4().hex[:8]}.vpk'
        dest.parent.mkdir(parents=True,exist_ok=True)
        shutil.copyfile(work/'candidate.vpk',dest)
        Path(str(dest)+'.sha256').write_text(sha(dest)+'  '+dest.name+'\n')
        shutil.copyfile(work/'package/build-manifest.json',Path(str(dest)+'.manifest.json'))
        print('Candidate:',dest)
        return dest
    except subprocess.CalledProcessError as e:
        raise RuntimeError(f'Build failed; inspect {work}/build.log. No candidate promoted; existing artifacts untouched.') from e
    finally: lock.rmdir()

def prepare_candidate(root, source_ref, fetch=True):
    clean(root)
    if fetch: git('fetch','origin',cwd=root)
    source=git('rev-parse','--verify','--end-of-options',source_ref+'^{commit}',cwd=root)
    previous=git('rev-parse','HEAD',cwd=root)
    old_manifest=json.loads((root/'personal-features.json').read_text())
    check_ids(root,source)
    branch='candidate/vita/'+datetime.datetime.now().strftime('%Y%m%d-%H%M%S')+'-'+uuid.uuid4().hex[:6]
    path=root.parent/('vita-update-'+branch.rsplit('/',1)[1])
    git('worktree','add','-b',branch,str(path),previous,cwd=root)
    record={'previous_vita_revision':previous,'previous_source_base':old_manifest['ports']['vita']['source_commit'],
            'selected_source_ref':source_ref,'selected_source_commit':source,'candidate_branch':branch,'worktree':str(path)}
    (path/'.tmp').mkdir(exist_ok=True)
    record_path=path/'.tmp/vita-update.json'
    record_path.write_text(json.dumps(record,indent=2)+'\n')
    try: git('merge','--no-edit',source,cwd=path)
    except subprocess.CalledProcessError:
        conflicts=git('diff','--name-only','--diff-filter=U',cwd=path)
        record['conflicts']=conflicts.splitlines()
        record_path.write_text(json.dumps(record,indent=2)+'\n')
        raise RuntimeError(f'Merge stopped in {path} on {branch}. Original branch/artifacts unchanged.\nConflicts:\n{conflicts}\nResolve there and commit, or leave candidate for review; no automatic resolution.')
    check_ids(path)
    manifest=json.loads((path/'personal-features.json').read_text())
    manifest['ports']['vita']['source_commit']=source
    manifest['ports']['vita']['previous_vita_revision']=previous
    (path/'personal-features.json').write_text(json.dumps(manifest,indent=2)+'\n')
    if git('status','--porcelain',cwd=path):
        git('add','personal-features.json',cwd=path)
        git('commit','-m','Record personal source for Vita update candidate',cwd=path)
    (path/'.tmp').mkdir(exist_ok=True)
    record['candidate_revision']=git('rev-parse','HEAD',cwd=path)
    record_path.write_text(json.dumps(record,indent=2)+'\n')
    print('Prepared candidate:',path)
    return path

def update(root, source_ref, profile, fetch=True):
    candidate=prepare_candidate(root,source_ref,fetch)
    # Execute the merged script, so subsequent port updates take effect too.
    run([candidate/"scripts/vita.sh","build","--profile",profile],candidate)
    return candidate

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    commands=parser.add_subparsers(dest='command',required=True)
    commands.add_parser('doctor')
    for name in ('build','update'):
        p=commands.add_parser(name); p.add_argument('--profile',choices=('ordinary','personal'),default='ordinary')
        if name=='update': p.add_argument('--source-ref',required=True)
    p=commands.add_parser('verify'); p.add_argument('--artifact',required=True); p.add_argument('--profile',choices=('ordinary','personal'))
    args=parser.parse_args()
    if args.command=='doctor': doctor()
    elif args.command=='verify': verify(args.artifact,args.profile)
    elif args.command=='build': build(ROOT,args.profile)
    else: update(ROOT,args.source_ref,args.profile)

if __name__=='__main__':
    try: main()
    except (RuntimeError,subprocess.CalledProcessError,ValueError,KeyError,OSError,zipfile.BadZipFile) as e:
        sys.exit('Vita: '+str(e))
