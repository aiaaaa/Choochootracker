"""Deterministic workflow tests in disposable repositories; no SDK/network/music."""
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest
import zipfile

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

if __name__=='__main__': unittest.main()
