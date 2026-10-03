"""Deterministic workflow tests in disposable repositories; no SDK/network/music."""
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import struct
import subprocess
import tempfile
import unittest
import zipfile
from vita_assets import IMAGES, validate_livearea, validate_png
from vita_elf import validate_relocations, validate_threads

spec=importlib.util.spec_from_file_location('vita',Path(__file__).with_name('vita.py'))
vita=importlib.util.module_from_spec(spec); spec.loader.exec_module(vita)

class Workflow(unittest.TestCase):
    def setUp(self):
        self.temp=tempfile.TemporaryDirectory(prefix='cct-vita-workflow-')
        self.parent=Path(self.temp.name); self.root=self.parent/'repo'; self.root.mkdir()
        self.git('init','-b','source'); self.git('config','user.name','Vita workflow test')
        self.git('config','user.email','vita-test@localhost')
        (self.root/'chipnomad_lib').mkdir()
        shutil.copy(vita.ROOT/'chipnomad_lib/project_instruments.h',self.root/'chipnomad_lib')
        (self.root/'.gitignore').write_text('.tmp/\nreleases/\n')
        (self.root/'shared.txt').write_text('base\n'); self.commit('initial')
        self.source=self.git('rev-parse','HEAD')
        self.git('checkout','-b','personal/vita')
        (self.root/'personal-features.json').write_text(json.dumps({'ports':{'vita':{'source_commit':self.source}}}))
        self.commit('port'); self.previous=self.git('rev-parse','HEAD')
        self.good=self.root/'releases/last-known-good.vpk'; self.good.parent.mkdir()
        self.good.write_bytes(b'known-good'); self.good_hash=vita.sha(self.good)

    def tearDown(self): self.temp.cleanup()
    def git(self,*args): return vita.git(*args,cwd=self.root)
    def commit(self,message): self.git('add','.'); self.git('commit','-m',message)
    def preserved(self):
        self.assertEqual(self.git('rev-parse','personal/vita'),self.previous)
        self.assertEqual(vita.sha(self.good),self.good_hash)
        self.assertEqual(self.git('status','--porcelain'),'')

    def test_new_revision_and_repeat_do_not_duplicate_feature_merges(self):
        self.git('checkout','source'); (self.root/'new-feature.txt').write_text('new personal feature\n')
        self.commit('new personal revision'); new=self.git('rev-parse','HEAD')
        self.git('checkout','personal/vita')
        candidate=vita.prepare_candidate(self.root,new,False)
        self.assertTrue((candidate/'new-feature.txt').exists())
        self.assertEqual(json.loads((candidate/'personal-features.json').read_text())['ports']['vita']['source_commit'],new)
        again=vita.prepare_candidate(candidate,new,False)
        commits=vita.git('log','--format=%s',cwd=again).splitlines()
        self.assertEqual(commits.count('new personal revision'),1)
        self.preserved()
        # A failed command in the candidate cannot alter the original/LKG.
        with self.assertRaises(subprocess.CalledProcessError): vita.run(['sh','-c','exit 9'],candidate)
        self.preserved()

    def test_merge_conflict_keeps_original_branch_and_artifact(self):
        (self.root/'shared.txt').write_text('vita change\n'); self.commit('vita edit')
        self.previous=self.git('rev-parse','HEAD')
        self.git('checkout','source'); (self.root/'shared.txt').write_text('personal change\n'); self.commit('source edit')
        source=self.git('rev-parse','HEAD'); self.git('checkout','personal/vita')
        with self.assertRaisesRegex(RuntimeError,'Conflicts:.*',): vita.prepare_candidate(self.root,source,False)
        self.preserved()

    def test_failed_candidate_build_keeps_last_known_good(self):
        (self.root/'scripts').mkdir()
        launcher=self.root/'scripts/vita.sh'
        launcher.write_text('#!/bin/sh\nexit 23 # controlled build failure\n'); launcher.chmod(0o755)
        self.commit('controlled build failure'); self.previous=self.git('rev-parse','HEAD')
        with self.assertRaises(subprocess.CalledProcessError):
            vita.update(self.root,self.source,'personal',False)
        self.preserved()

    def test_dirty_source_refuses_without_new_worktree(self):
        (self.root/'shared.txt').write_text('unsaved\n')
        with self.assertRaisesRegex(RuntimeError,'dirty'): vita.prepare_candidate(self.root,self.source,False)
        self.assertEqual(len(list(self.parent.glob('vita-update-*'))),0)

    def test_collision_fails_before_merge(self):
        p=self.root/'chipnomad_lib/project_instruments.h'
        p.write_text(p.read_text().replace('FrontTouch = 8','UpstreamOtherSource = 8'))
        self.commit('collision')
        with self.assertRaisesRegex(RuntimeError,'collision'): vita.check_ids(self.root)

    def test_package_corruption_is_not_silently_accepted(self):
        package=self.root/'bad.vpk'; package.write_bytes(b'broken archive')
        Path(str(package)+'.sha256').write_text('0'*64+'  bad.vpk\n')
        with self.assertRaisesRegex(RuntimeError,'checksum'): vita.verify(package)

    def test_package_inventory_profile_identity_and_asset_hashes(self):
        # Self-authored structural fixture, not a runnable Vita executable.
        title=vita.pin()['title_id'].encode()+b'\0'
        keys=b'TITLE_ID\0'
        sfo=struct.pack('<5I',0x46535000,0x101,36,36+len(keys),1)
        sfo+=struct.pack('<HHIII',0,0x204,len(title),len(title),0)+keys+title
        files={'eboot.bin':b'SCE\0fixture','sce_sys/param.sfo':sfo,
               'VITA.md':b'fixture',
               'licenses/ChooChooTracker.md':b'fixture'}
        for name in (*IMAGES, 'sce_sys/livearea/contents/template.xml'):
            files[name]=(vita.ROOT/'tracker/packaging/vita'/name).read_bytes()
        for folder in ('fonts','title','projects','pitch-tables'):
            files['assets/'+folder+'/fixture']=b'fixture'
        manifest={'profile':'ordinary','flags':{'CHOOCHOO_EXPERIMENTAL_MOD_LUCKY':0},
                  'validation':{'elf_relocations':{'.rel.text':1,'.rel.init_array':1},
                                'pthread_symbols':['pthread_cancel']},
                  'application_commit':self.previous,'personal_source_commit':self.source,
                  'vita_integration_revision':self.previous,
                  'files':{name:hashlib.sha256(data).hexdigest() for name,data in files.items()}}
        package=self.root/'structural-fixture.vpk'
        def write():
            with zipfile.ZipFile(package,'w') as z:
                for name,data in files.items(): z.writestr(name,data)
                z.writestr('build-manifest.json',json.dumps(manifest))
            Path(str(package)+'.sha256').write_text(vita.sha(package)+'  '+package.name+'\n')
        write()
        self.assertEqual(vita.verify(package,'ordinary')['application_commit'],self.previous)
        threads=manifest['validation'].pop('pthread_symbols')
        write()
        with self.assertRaisesRegex(RuntimeError,'pthread activation'): vita.verify(package)
        manifest['validation']['pthread_symbols']=threads
        write()
        with self.assertRaisesRegex(RuntimeError,'profile'): vita.verify(package,'personal')
        icon=files['sce_sys/icon0.png']
        files['sce_sys/icon0.png']=b'changed'
        write()
        with self.assertRaisesRegex(RuntimeError,'Asset checksum'): vita.verify(package)
        files['sce_sys/icon0.png']=icon
        manifest['application_commit']='moving-branch'
        write()
        with self.assertRaisesRegex(RuntimeError,'identity'): vita.verify(package)
        manifest['application_commit']=self.previous
        del files['assets/fonts/fixture']
        write()
        with self.assertRaisesRegex(RuntimeError,'Missing assets/fonts'): vita.verify(package)

    def test_livearea_rejects_installer_incompatible_pngs(self):
        root=vita.ROOT/'tracker/packaging/vita'
        validate_livearea(lambda name: (root/name).read_bytes())
        name='sce_sys/icon0.png'
        good=(root/name).read_bytes()
        for byte,value in ((24,16),(25,2),(25,6),(28,1)):
            bad=bytearray(good); bad[byte]=value
            with self.assertRaisesRegex(RuntimeError,'encoding'):
                validate_png(bytes(bad),name,(128,128))
        with self.assertRaisesRegex(RuntimeError,'dimensions'):
            validate_png(good,name,(960,544))
        with self.assertRaisesRegex(RuntimeError,'truncated|incomplete'):
            validate_png(good[:-8],name,(128,128))

    def test_missing_runtime_relocations_fail_packaging(self):
        report="Relocation section '.rel.text' at offset 0x123 contains 123 entries:\n"
        report+="Relocation section '.rel.init_array' at offset 0x345 contains 1 entry:\n"
        self.assertEqual(validate_relocations(report)['.rel.init_array'],1)
        for invalid in ('There are no relocations in this file.',
                        report.splitlines()[0], report.replace('123 entries','0 entries')):
            with self.assertRaisesRegex(RuntimeError,'missing code/constructor relocations'):
                validate_relocations(invalid)

    def test_weak_pthread_proxy_fails_packaging(self):
        symbols='812abc90 T pthread_create\n812ac32c T pthread_once\n'
        self.assertIn('pthread_cancel',validate_threads(symbols+'812ac990 T pthread_cancel\n', personal=True))
        self.assertEqual(validate_threads('812ac990 T pthread_cancel\n'),['pthread_cancel'])
        with self.assertRaisesRegex(RuntimeError,'pthread support'):
            validate_threads('812ac990 T pthread_cancel\n', personal=True)
        for missing in ('', '         w pthread_cancel\n', '         U pthread_cancel\n'):
            with self.assertRaisesRegex(RuntimeError,'pthread support'):
                validate_threads(symbols+missing, personal=True)

if __name__=='__main__': unittest.main()
