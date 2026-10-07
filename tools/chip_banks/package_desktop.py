#!/usr/bin/env python3
"""Package an existing macOS personal build and all runtime assets, without installing."""
import argparse,hashlib,json,os,shutil,subprocess,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
def preset_inventory(folder):
 """Validate every catalog reference and collection CRC before packaging."""
 entries=[]
 for catalog in ('catalog.tsv','builtins.tsv'):
  lines=(folder/catalog).read_text().splitlines()
  if lines[0]!='CCT-CHIP-CATALOG\t1':raise ValueError('invalid catalog '+catalog)
  for line in lines[1:]:
   fields=line.split('\t')
   if len(fields)!=7:raise ValueError('expected archived preset in '+catalog)
   entries.append((fields[6],fields[5]))
 if not entries or len(set(entries))!=len(entries):raise ValueError('empty or duplicate preset inventory')
 actual=set()
 for pack in folder.glob('*.zip'):
  with zipfile.ZipFile(pack) as z:
   bad=z.testzip()
   if bad:raise ValueError('collection CRC failure '+bad)
   actual.update((pack.name,n) for n in z.namelist() if n.endswith('.cni'))
 if actual!=set(entries):raise ValueError('collection/catalog inventory mismatch')
 return len(entries)

def package(build,stage,archive):
 if stage.exists():raise ValueError('stage must be a new directory')
 executable=build/'choochootracker';framework=build/'Frameworks/SDL2.framework'
 if not executable.is_file() or not framework.is_dir():raise ValueError('built executable and SDL2 framework required')
 stage.mkdir(parents=True)
 shutil.copy2(executable,stage/executable.name)
 for source in (ROOT/'tracker/packaging/common').iterdir():
  if source.is_dir():shutil.copytree(source,stage/source.name)
  elif source.is_file():shutil.copy2(source,stage/source.name)
 shutil.copytree(framework,stage/'Frameworks/SDL2.framework',symlinks=True)
 if (build/'licenses').is_dir():shutil.copytree(build/'licenses',stage/'licenses',dirs_exist_ok=True)
 shutil.copy2(ROOT/'tracker/packaging/portmaster/cover.png',stage/'personal-fork-cover.png')
 shutil.copy2(ROOT/'LICENSE',stage/'LICENSE.txt')
 for filename in ('USER_MANUAL.md','chip-instruments-report.md','chip-preset-auditions.tsv','chip-r36h-benchmark.csv','chip-r36h-soak.csv','chip-r36h-validation.json'):
  shutil.copy2(ROOT/'docs'/filename,stage/filename)
 for filename in ('PERSONAL_FORK.md','personal-features.json'):
  shutil.copy2(ROOT/filename,stage/filename)
 (stage/'START-HERE.txt').write_text('ChooChooTracker native chip development build — macOS x86_64\n\nRun ./choochootracker from this directory. All banks and notices are included.\nThe personal Mod Lucky experiment is enabled. No user settings or songs were\ncollected. This archive does not install or update the handheld.\nRead chip-instruments-report.md for tests, measured R36H limits and remaining listening checks.\n')
 preset_count=preset_inventory(stage/'instruments/FACTORY')
 for required in ('msfa/LICENSE','msfa/NOTICE','ymfm/LICENSE','emu76489/LICENSE','gb_apu/LICENCE.txt','gb_apu/Blip_Buffer.txt','miniz.txt'):
  if not (stage/'licenses'/required).is_file():raise ValueError('missing notice '+required)
 hashes={str(p.relative_to(stage)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(stage.rglob('*')) if p.is_file() and not p.is_symlink()}
 (stage/'package-manifest.json').write_text(json.dumps(dict(schema=1,platform='macOS x86_64',source_commit=subprocess.check_output(['git','rev-parse','HEAD'],cwd=ROOT,text=True).strip(),personal_mod_lucky=True,preset_count=preset_count,files=hashes),indent=2)+'\n')
 archive.parent.mkdir(parents=True,exist_ok=True)
 if archive.exists():raise ValueError('archive already exists')
 subprocess.run(['zip','-qry',str(archive),stage.name],cwd=stage.parent,check=True)
 with zipfile.ZipFile(archive) as z:
  bad=z.testzip()
  if bad:raise ValueError('archive CRC failure '+bad)
  for name,digest in hashes.items():
   if hashlib.sha256(z.read(stage.name+'/'+name)).hexdigest()!=digest:raise ValueError('archive inventory '+name)
 print(json.dumps(dict(archive=str(archive),bytes=archive.stat().st_size,sha256=hashlib.sha256(archive.read_bytes()).hexdigest(),native_presets=preset_count)))
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--build',type=Path,required=True);p.add_argument('--stage',type=Path,required=True);p.add_argument('--archive',type=Path,required=True);a=p.parse_args();package(a.build.resolve(),a.stage.resolve(),a.archive.resolve())
