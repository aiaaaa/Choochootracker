import io
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
import zipfile

from migrate_legacy_beads import migrate_bytes

TRACK = b"- Insert: 0,1,14,1,0,255,179,128,0,0,38,255\r\n"
MASTER = b"- Master insert: 1,14,0,26,191,179,192,0,0,38,128\r\n"
SONG = b"# ChooChooTracker Module 9.0\r\n\r\n" + TRACK + MASTER + b"- Title: 14,15\r\n"


class MigrationTests(unittest.TestCase):
    def test_only_module_fields_change(self):
        converted, changes = migrate_bytes(SONG, legacy_personal=True)
        self.assertEqual(converted, SONG.replace(b"0,1,14,", b"0,1,15,").replace(b"1,14,0,", b"1,15,0,"))
        self.assertEqual(len(changes), 2)

    def test_requires_explicit_legacy_provenance(self):
        with self.assertRaises(ValueError):
            migrate_bytes(SONG)

    def test_rejects_double_conversion(self):
        converted, _ = migrate_bytes(SONG, legacy_personal=True)
        with self.assertRaises(ValueError):
            migrate_bytes(converted, legacy_personal=True)

    def test_other_modules_unchanged(self):
        old = SONG.replace(b",14,", b",13,")
        self.assertEqual(migrate_bytes(old, legacy_personal=True), (old, []))

    def test_archive_preserves_samples_names_comment_and_storage(self):
        buffer = io.BytesIO()
        sample = bytes(range(256)) * 32
        with zipfile.ZipFile(buffer, "w") as archive:
            archive.comment = b"original comment"
            archive.writestr("project.cct", SONG)
            archive.writestr("samples/000.wav", sample)
        converted, changes = migrate_bytes(buffer.getvalue(), legacy_personal=True)
        with zipfile.ZipFile(io.BytesIO(converted)) as archive:
            self.assertIsNone(archive.testzip())
            self.assertEqual(archive.read("samples/000.wav"), sample)
            self.assertEqual(archive.comment, b"original comment")
            self.assertEqual(archive.namelist(), ["project.cct", "samples/000.wav"])
            self.assertTrue(all(e.compress_type == zipfile.ZIP_STORED for e in archive.infolist()))
            self.assertEqual(len(changes), 2)

    def test_malformed_input_rejected(self):
        for data in (b"not a project", SONG.replace(b",255,179,", b",256,179,"),
                     SONG.replace(b",14,1,", b",14,2,"), SONG.replace(b",14,", b",wat,")):
            with self.subTest(data=data), self.assertRaises(ValueError):
                migrate_bytes(data, legacy_personal=True)

    def test_cli_never_overwrites(self):
        script = Path(__file__).with_name("migrate_legacy_beads.py")
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / "song.cct"
            source.write_bytes(SONG)
            result = subprocess.run([sys.executable, str(script), str(source), "--legacy-personal",
                                     "--output", str(source)], capture_output=True)
            self.assertNotEqual(result.returncode, 0)
            self.assertEqual(source.read_bytes(), SONG)


if __name__ == "__main__":
    unittest.main()
