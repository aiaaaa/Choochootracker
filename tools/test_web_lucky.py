#!/usr/bin/env python3
"""Optional browser regression test; needs Playwright, Chrome and a C++ compiler.
Run after a fresh web build. No archive traffic or downloaded music is used.
CHROME_EXECUTABLE can select an existing Chrome installation.
"""
import functools,http.server,threading,pathlib,os,subprocess,tempfile
from playwright.sync_api import sync_playwright
root=pathlib.Path(__file__).resolve().parents[1]
class Handler(http.server.SimpleHTTPRequestHandler):
    def log_message(self,*args):pass
server=http.server.ThreadingHTTPServer(('127.0.0.1',0),functools.partial(Handler,directory=str(root/'web/dist')))
threading.Thread(target=server.serve_forever,daemon=True).start()
with tempfile.TemporaryDirectory() as directory:
    source=pathlib.Path(directory)/'fixture.cpp'
    source.write_text('#include <cstdint>\n#include "mod_lucky_fixtures.h"\n#include <cstdio>\nint main(){auto b=luckyFixture::mod();return fwrite(b.data(),1,b.size(),stdout)==b.size()?0:1;}\n')
    executable=str(pathlib.Path(directory)/'fixture')
    subprocess.run([os.environ.get('CXX','c++'),'-std=c++17','-I'+str(root/'tracker/tests'),str(source),'-o',executable],check=True)
    fixture=subprocess.check_output([executable])
(root/'.tmp').mkdir(exist_ok=True)
with sync_playwright() as p:
    browser=p.chromium.launch(executable_path=os.environ.get('CHROME_EXECUTABLE','/Applications/Google Chrome.app/Contents/MacOS/Google Chrome'),headless=True)
    try:
        page=browser.new_page(); errors=[]; requests=[]; scenario={'mode':'valid','id':100,'pending':[]}
        page.on('pageerror',lambda e:errors.append(str(e)))
        page.on('console',lambda m:print('console',m.type,m.text[:150],flush=True) if m.type=='error' else None)
        def route_api(route):
            requests.append('page')
            if scenario['mode']=='delay':scenario['pending'].append(route);return
            if scenario['mode']=='limit':route.fulfill(status=429,headers={'Retry-After':'60'},body='limited');return
            scenario['id']+=1;n=scenario['id']
            route.fulfill(content_type='text/plain',body=f'''path="jsplayer.php?moduleid={n}"; href="https://api.modarchive.org/downloads.php?moduleid={n}#fixture.mod"''')
        def route_module(route):
            requests.append('module');route.fulfill(content_type='application/octet-stream',headers={'Access-Control-Allow-Origin':'*'},body=fixture)
        page.route('**/api/lucky',route_api);page.route('https://api.modarchive.org/downloads.php?*',route_module)
        def ready():page.wait_for_function("window.Module && typeof calledRun!=='undefined' && calledRun && !document.querySelector('#import-project').disabled",timeout=45000)
        page.goto(f'http://127.0.0.1:{server.server_port}/');ready()
        page.locator('#canvas').click()
        def key(k):page.keyboard.press(k,delay=100);page.wait_for_timeout(180)
        key('x')
        page.evaluate("window.probe={callbacks:0,peak:0};const n=Module.SDL2.audio.scriptProcessorNode;const f=n.onaudioprocess;n.onaudioprocess=e=>{f(e);probe.callbacks++;for(const x of e.outputBuffer.getChannelData(0))probe.peak=Math.max(probe.peak,Math.abs(x));};void 0;")
        key('Shift+ArrowDown')
        for _ in range(9):key('ArrowDown')
        page.screenshot(path=str(root/'.tmp/lucky-before.png'))
        key('x');page.wait_for_timeout(1800)
        assert requests==['page','module'],requests
        assert page.evaluate('probe.peak')==0,'NEXT auto-played'
        page.screenshot(path=str(root/'.tmp/lucky-ready.png'))
        key('x');page.wait_for_timeout(1000)
        print('PLAY audio',page.evaluate('probe'),flush=True)
        assert page.evaluate('probe.peak')>0
        key('ArrowRight');key('x');page.wait_for_timeout(800)
        page.screenshot(path=str(root/'.tmp/lucky-loaded.png'))
        files=page.evaluate("""()=>{const files=[];function scan(p){for(const n of FS.readdir(p)){if(n==='.'||n==='..')continue;const q=p+'/'+n;if(FS.isDir(FS.stat(q).mode))scan(q);else files.push([q,FS.stat(q).size]);}}scan('/user/samples');return files;}""")
        print('imported files',files,flush=True);assert sum(n.endswith('.wav') for n,s in files)==4
        assert requests==['page','module'],'LOAD downloaded again'
        assert page.evaluate("Module.ccall('webSaveProject','number',['string'],['/user/projects/lucky-test.cct'])")==0
        page.evaluate("new Promise((resolve,reject)=>FS.syncfs(false,e=>e?reject(e):resolve()))")
        page.reload();ready()
        assert page.evaluate("Module.ccall('webLoadProject','number',['string'],['/user/projects/lucky-test.cct'])")==0
        for name,size in files:assert page.evaluate('p=>FS.stat(p).size',name)==size
        print('project and sample persistence passed',flush=True)
        # Leave while the random page is pending. It must not complete or play later.
        page.locator('#canvas').click();key('x');key('Shift+ArrowDown')
        for _ in range(9):key('ArrowDown')
        scenario['mode']='delay';key('x');assert scenario['pending']
        key('z');page.wait_for_timeout(300)
        for route in scenario['pending']:
            try:route.abort()
            except Exception:pass
        key('Shift+ArrowDown');scenario['mode']='limit';before=len(requests);key('x');page.wait_for_timeout(400)
        assert len(requests)==before+1,'NEXT remained blocked after cancellation'
        before=len(requests);key('x');page.wait_for_timeout(300);assert len(requests)==before,'rate limit bypassed'
        page.screenshot(path=str(root/'.tmp/lucky-rate-limit.png'))
        print('cancellation, rate limit and errors',errors,flush=True);assert not errors
    finally:browser.close();server.shutdown()
