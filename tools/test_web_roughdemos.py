#!/usr/bin/env python3
"""Check bundled device songs and non-destructive browser seeding in isolated Chrome.
Uses the same optional Playwright environment as test_web_presentation.py.
Set WEB_THEME_TEST_URL to verify the deployed collection instead of local dist.
"""
import base64
import functools
import hashlib
import http.server
import io
import json
import os
from pathlib import Path
import threading
import zipfile

from playwright.sync_api import sync_playwright

root = Path(__file__).resolve().parents[1]
collection = root / 'tracker/packaging/web/roughdemos'
records = json.loads((collection / 'collection.json').read_text())['projects']


class Handler(http.server.SimpleHTTPRequestHandler):
    def log_message(self, *args):
        pass


server = http.server.ThreadingHTTPServer(
    ('127.0.0.1', 0), functools.partial(Handler, directory=str(root / 'web/dist')))
threading.Thread(target=server.serve_forever, daemon=True).start()
url = os.environ.get('WEB_THEME_TEST_URL', f'http://127.0.0.1:{server.server_port}/')
with sync_playwright() as playwright:
    browser = playwright.chromium.launch(
        executable_path=os.environ.get('CHROME_EXECUTABLE', '/Applications/Google Chrome.app/Contents/MacOS/Google Chrome'),
        headless=True)
    try:
        page = browser.new_page()
        errors = []
        page.on('pageerror', lambda error: errors.append(str(error)))

        def ready():
            page.wait_for_function("typeof calledRun!=='undefined' && calledRun && !document.querySelector('#import-project').disabled", timeout=60000)
            assert 'Could not' not in page.locator('#status').inner_text()

        def read(path):
            return base64.b64decode(page.evaluate("""path => {
                const bytes=FS.readFile(path); let binary='';
                for(let i=0;i<bytes.length;i+=32768) binary+=String.fromCharCode(...bytes.subarray(i,i+32768));
                return btoa(binary);
            }""", path))

        def digest(path):
            return hashlib.sha256(read(path)).hexdigest()

        def flush():
            page.evaluate("() => new Promise((resolve,reject) => syncUserStorage(error => error ? reject(error) : resolve()))")

        page.goto(url)
        ready()
        page.locator('#canvas').focus()
        assert page.evaluate("getComputedStyle(document.querySelector('#canvas')).outlineStyle") == 'none'
        assert not page.evaluate("FS.analyzePath('/bundled-projects').exists")
        assert page.evaluate("Module.ccall('webSaveBrowserState','number',[],[])") == 0
        assert b'bootScreenNight: 1\n' in read('/user/settings.txt')
        for record in records:
            path = '/user/projects/rough demos/' + record['file']
            assert digest(path) == record['sha256'], path
            assert page.evaluate("path => Module.ccall('webLoadProject','number',['string'],[path])", path) == 0, path
            assert page.evaluate("Module.ccall('webSaveProject','number',['string'],['/user/exports/roundtrip.cct'])") == 0
            saved = read('/user/exports/roundtrip.cct')
            embedded = {}
            if zipfile.is_zipfile(io.BytesIO(saved)):
                with zipfile.ZipFile(io.BytesIO(saved)) as archive:
                    assert archive.testzip() is None
                    embedded = {name: archive.read(name) for name in archive.namelist() if name.startswith('samples/')}
            assert len(embedded) == record['embedded_samples'], path
            if embedded:
                with zipfile.ZipFile(collection / record['file']) as original:
                    assert embedded == {name: original.read(name) for name in original.namelist() if name.startswith('samples/')}, path
        print(f"All {len(records)} seeded projects loaded and saved with every embedded sample intact", flush=True)

        # Save a different valid project over a seeded filename. A later load
        # must preserve this edit as well as Continue and private preset data.
        changed = '/user/projects/rough demos/' + records[0]['file']
        assert page.evaluate("Module.ccall('webLoadProject','number',['string'],['/user/projects/rough demos/Left-Right-Stereo-Test.cct'])") == 0
        assert page.evaluate("path => Module.ccall('webSaveProject','number',['string'],[path])", changed) == 0
        assert page.evaluate("Module.ccall('webSaveBrowserState','number',[],[])") == 0
        changed_hash = digest(changed)
        assert changed_hash != records[0]['sha256']
        continue_hash = digest('/user/autosave.cct')
        page.evaluate("FS.mkdirTree('/user/instruments/USER/sid'); FS.writeFile('/user/instruments/USER/sid/test-user-data.txt','keep private');")
        flush()
        page.reload()
        ready()
        assert digest(changed) == changed_hash
        assert digest('/user/autosave.cct') == continue_hash
        assert read('/user/instruments/USER/sid/test-user-data.txt') == b'keep private'
        assert digest('/user/projects/rough demos/' + records[-1]['file']) == records[-1]['sha256']
        # Model an older profile with edited demos and an Original boot choice.
        # The folder moves without losing edits, and Night applies only once.
        page.evaluate("""() => {
            FS.rename('/user/projects/rough demos', '/user/projects/roughdemos');
            let settings = FS.readFile('/user/settings.txt', {encoding:'utf8'});
            settings = settings.replace(/^personalNightDefault:.*\\n/m, '')
                .replace(/^bootScreenNight:.*$/m, 'bootScreenNight: 0')
                .replace(/^projectPath:.*$/m, 'projectPath: /user/projects/roughdemos');
            FS.writeFile('/user/settings.txt', settings);
        }""")
        flush()
        page.reload()
        ready()
        assert not page.evaluate("FS.analyzePath('/user/projects/roughdemos').exists")
        assert digest(changed) == changed_hash
        settings = read('/user/settings.txt')
        assert b'bootScreenNight: 1\n' in settings
        assert b'personalNightDefault: 1\n' in settings
        assert b'projectPath: /user/projects/rough demos\n' in settings
        page.evaluate("""() => {
            let settings = FS.readFile('/user/settings.txt', {encoding:'utf8'});
            FS.writeFile('/user/settings.txt', settings.replace(/^bootScreenNight:.*$/m, 'bootScreenNight: 0'));
        }""")
        flush()
        page.reload()
        ready()
        assert b'bootScreenNight: 0\n' in read('/user/settings.txt')
        assert digest(changed) == changed_hash
        assert not errors, errors
        print('Frameless canvas, one-time Night default, legacy folder migration and later Original choice passed; edited songs, Continue and private data preserved', flush=True)
    finally:
        browser.close()
        server.shutdown()
