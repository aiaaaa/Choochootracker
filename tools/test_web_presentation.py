#!/usr/bin/env python3
"""Optional browser integration check after a web build.
Needs an existing Playwright install and Chrome. Set CHROME_EXECUTABLE to select
Chrome, or WEB_THEME_TEST_URL to check a deployed build in an isolated profile.
Phone lock/unlock is simulated here; it does not establish physical iOS behavior.
"""
import functools,http.server,threading,pathlib,os,json,base64
from playwright.sync_api import sync_playwright
root=pathlib.Path(__file__).resolve().parents[1]
class Handler(http.server.SimpleHTTPRequestHandler):
 def log_message(self,*args):pass
server=http.server.ThreadingHTTPServer(('127.0.0.1',0),functools.partial(Handler,directory=str(root/'web/dist')))
threading.Thread(target=server.serve_forever,daemon=True).start()
url=os.environ.get('WEB_THEME_TEST_URL',f'http://127.0.0.1:{server.server_port}/')
with sync_playwright() as p:
 browser=p.chromium.launch(executable_path=os.environ.get('CHROME_EXECUTABLE','/Applications/Google Chrome.app/Contents/MacOS/Google Chrome'),headless=True)
 try:
  errors=[]
  context=browser.new_context(viewport={'width':1512,'height':900},device_scale_factor=2)
  page=context.new_page();page.on('pageerror',lambda e:errors.append(str(e)))
  def ready(page):page.wait_for_function("window.Module && typeof calledRun!=='undefined' && calledRun && !document.querySelector('#import-project').disabled && document.documentElement.style.getPropertyValue('--background')",timeout=60000)
  page.goto(url);ready(page);page.locator('#canvas').click()
  # Fetch/decode the actual linked icons and install metadata, locally or live.
  icons=page.evaluate("""async ()=>{
    const sizes={};
    for(const rel of ['icon','apple-touch-icon']) {
      const img=new Image();img.src=document.querySelector('link[rel="'+rel+'"]').href;
      await img.decode();sizes[rel]=[img.naturalWidth,img.naturalHeight];
    }
    const response=await fetch(document.querySelector('link[rel="manifest"]').href);
    if(!response.ok)throw new Error('Manifest request failed');
    return {sizes,manifest:await response.json()};
  }""")
  assert icons['sizes']=={'icon':[128,128],'apple-touch-icon':[1254,1254]},icons
  assert icons['manifest']['display']=='standalone' and icons['manifest']['start_url']=='./',icons
  print('Browser and Home Screen icons decoded; standalone manifest passed',flush=True)
  def key(k):page.keyboard.press(k,delay=90);page.wait_for_timeout(300)
  key('x');key('Shift+ArrowDown')
  for _ in range(7):key('ArrowDown')
  key('x');key('x')
  page.screenshot(path=str(root/'.tmp/theme-before-edit.png'))
  before=page.evaluate("document.documentElement.style.getPropertyValue('--background')")
  key('x+ArrowRight')
  page.wait_for_timeout(500)
  after=page.evaluate("document.documentElement.style.getPropertyValue('--background')")
  assert before!=after,(before,after)
  assert page.locator('meta[name="theme-color"]').get_attribute('content')==after
  settings=page.evaluate("FS.readFile('/user/settings.txt',{encoding:'utf8'})")
  assert 'colorBackground: 0x'+after[1:] in settings
  page.reload();ready(page)
  assert page.evaluate("document.documentElement.style.getPropertyValue('--background')")==after
  print('Live edited color and reload persistence passed:',before,after,flush=True)
  # Load an actual theme using the same native settings screen.
  page.locator('#canvas').click();key('x');key('Shift+ArrowDown')
  for _ in range(7):key('ArrowDown')
  key('x');key('x')
  for _ in range(11):key('ArrowDown')
  key('ArrowRight');key('x');page.screenshot(path=str(root/'.tmp/theme-file-picker.png'))
  # File browser has parent entry, IDE folder, then alphabetically sorted files.
  for _ in range(5):key('ArrowDown')
  key('x');page.wait_for_timeout(600)
  page.screenshot(path=str(root/'.tmp/theme-loaded.png'))
  print('Selected theme settings:',page.evaluate("FS.readFile('/user/settings.txt',{encoding:'utf8'}).split('\\n').filter(x=>x.startsWith('themeName:')||x.startsWith('colorBackground:'))"),flush=True)
  assert 'themeName: NostromoAmberDa2' in page.evaluate("FS.readFile('/user/settings.txt',{encoding:'utf8'})")
  colors=page.evaluate("Object.fromEntries(['background','text','value','title','cursor','selection','warning'].map(n=>[n,getComputedStyle(document.documentElement).getPropertyValue('--'+n)]))")
  print('Active palette',colors,flush=True)
  page.reload();ready(page)
  assert page.evaluate("getComputedStyle(document.documentElement).getPropertyValue('--background')")==colors['background']
  page.locator('#canvas').click();key('x');page.wait_for_timeout(500)
  # Every visible surrounding element follows the viewing surface in the page.
  def geometry(page):return page.evaluate("()=>{const c=document.querySelector('#canvas'),r=c.getBoundingClientRect(),d=devicePixelRatio;return {w:r.width,h:r.height,scale:r.width*d/c.width,x:r.x*d,bottom:r.bottom,below:[...document.querySelectorAll('#browser-controls,#touch-controls')].filter(e=>getComputedStyle(e).display!=='none').every(e=>e.getBoundingClientRect().top>=r.bottom),background:getComputedStyle(document.body).backgroundColor};}")
  g=geometry(page);assert abs(g['scale']*2-round(g['scale']*2))<.01 and g['below'],g
  def check_font_pixels(page):
   png=page.screenshot(scale='device',full_page=True)
   result=page.evaluate("""async png=>{
     const img=new Image();img.src='data:image/png;base64,'+png;await img.decode();
     const ctx=document.createElement('canvas').getContext('2d');
     ctx.canvas.width=img.width;ctx.canvas.height=img.height;ctx.drawImage(img,0,0);
     const r=document.querySelector('#canvas').getBoundingClientRect(),d=devicePixelRatio;
     const scale=r.width*d/640,unit=Math.round(scale*2);
     // SONG is four 16x24 glyphs at row 2 when the waveform is enabled.
     const w=Math.round(64*scale),h=Math.round(24*scale);
     const data=ctx.getImageData(Math.round(r.left*d),Math.round(r.top*d+48*scale),w,h).data;
     const color=parseInt(getComputedStyle(document.documentElement).getPropertyValue('--title').trim().slice(1),16);
     const ink=(x,y)=>{const i=(y*w+x)*4;return data[i]===(color>>16&255)&&data[i+1]===(color>>8&255)&&data[i+2]===(color&255);};
     let runs=0,uneven=0;
     for(const horizontal of [true,false])for(let a=0;a<(horizontal?h:w);a++){
       let length=0;for(let b=0;b<=(horizontal?w:h);b++){
         if(b<(horizontal?w:h)&&(horizontal?ink(b,a):ink(a,b)))length++;
         else if(length){runs++;if(length%unit)uneven++;length=0;}
       }
     }
     return {unit,runs,uneven};
   }""",base64.b64encode(png).decode())
   assert result['runs']>50 and result['uneven']==0,result
   print('Uniform Departure Mono stroke pixels',result,flush=True)
  check_font_pixels(page)
  print('Retina layout',g,flush=True);page.screenshot(path=str(root/'.tmp/theme-desktop.png'),full_page=True)
  page.set_viewport_size({'width':1100,'height':700});page.wait_for_timeout(250);g=geometry(page);assert abs(g['scale']*2-round(g['scale']*2))<.01 and g['below'],g
  print('Resized layout',g,flush=True)
  for width,height,dpr in [(393,852,3),(1440,900,1),(1024,768,1.25)]:
   c=browser.new_context(viewport={'width':width,'height':height},device_scale_factor=dpr,is_mobile=dpr==3,has_touch=dpr==3)
   t=c.new_page();t.goto(url);ready(t);g=geometry(t);assert abs(g['scale']*2-round(g['scale']*2))<.01 and g['below'],g
   print('Layout',width,height,dpr,g,flush=True)
   if dpr==3:
    t.locator('[data-button="4"]').tap();t.wait_for_timeout(500);t.screenshot(path=str(root/'.tmp/theme-phone.png'),full_page=True)
    assert abs(g['w']-320)<.01 and abs(g['h']-240)<.01,g
    assert t.locator('#touch-controls').bounding_box()['y'] >= t.locator('#browser-controls').bounding_box()['y'] + t.locator('#browser-controls').bounding_box()['height']
    assert t.evaluate("document.querySelector('#screen-area').scrollWidth===document.querySelector('#screen-area').clientWidth")
    check_font_pixels(t)
    # Model camera/Home-indicator insets without claiming to emulate iOS itself.
    t.evaluate("document.documentElement.style.setProperty('--safe-top','59px');document.documentElement.style.setProperty('--safe-bottom','34px');window.dispatchEvent(new Event('resize'))")
    t.wait_for_timeout(150)
    assert t.locator('#canvas').bounding_box()['y']>=67
    assert abs(geometry(t)['w']-320)<.01
    t.screenshot(path=str(root/'.tmp/home-screen-safe-area.png'),full_page=True)
    print('Simulated camera/Home-indicator safe-area layout passed',flush=True)
    # An ordinary, non-doubled font must return to whole canvas scale steps.
    t.evaluate("""()=>new Promise((resolve,reject)=>{let s=FS.readFile('/user/settings.txt',{encoding:'utf8'});s=s.replace(/^fontPath:.*$/m,'fontPath: /fonts/RobotoMono.cnfont');FS.writeFile('/user/settings.txt',s);FS.syncfs(false,e=>e?reject(e):resolve());})""")
    t.reload();ready(t)
    assert t.evaluate('fontPixelUnit')==1
    assert abs(geometry(t)['scale']-1)<.01
    print('Non-doubled font fallback passed',flush=True)

   c.close()
  # Exercise background save/pause/wake and gesture recovery without reloading.
  page.locator('#canvas').click()
  assert page.evaluate("Module.ccall('webLoadProject','number',['string'],['/projects/native-chip-audition.cct'])")==0
  page.evaluate("window.probe={callbacks:0,peak:0};const node=Module.SDL2.audio.scriptProcessorNode;const original=node.onaudioprocess;node.onaudioprocess=e=>{original(e);probe.callbacks++;for(const value of e.outputBuffer.getChannelData(0))probe.peak=Math.max(probe.peak,Math.abs(value));};void 0;")
  key('Space');page.wait_for_timeout(600);assert page.evaluate('probe.peak')>0
  for cycle in range(3):
   page.keyboard.down('ArrowRight');page.wait_for_timeout(100)
   page.evaluate("window.testHidden=true;Object.defineProperty(document,'hidden',{configurable:true,get:()=>window.testHidden});document.dispatchEvent(new Event('visibilitychange'));")
   page.wait_for_function("Module.SDL2.audioContext.state==='suspended'")
   assert page.evaluate("FS.stat('/user/autosave.cct').size")>0
   page.evaluate("testHidden=false;document.dispatchEvent(new Event('visibilitychange'));")
   page.keyboard.up('ArrowRight');page.locator('#canvas').click()
   page.wait_for_function("Module.SDL2.audioContext.state==='running'")
   page.wait_for_timeout(300);page.evaluate('probe.peak=0');key('Space');page.wait_for_timeout(600)
   assert page.evaluate('probe.peak')>0,('no playback after wake',cycle)
  page.evaluate("delete document.hidden")
  page.evaluate("Module.SDL2.audioContext.suspend()")
  page.locator('#canvas').click();page.wait_for_function("Module.SDL2.audioContext.state==='running'")
  print('Three background/save/wake cycles and gesture audio recovery passed',flush=True)
  # Preserve actual song bytes across replacing the browser audio output.
  def project_hash():
   assert page.evaluate("Module.ccall('webSaveBrowserState','number',[],[])")==0
   return page.evaluate("async()=>Array.from(new Uint8Array(await crypto.subtle.digest('SHA-256',FS.readFile('/user/autosave.cct')))).map(x=>x.toString(16).padStart(2,'0')).join('')")
  page.wait_for_function('!audioRecovering')
  before_restart=project_hash()
  # Inject a running clock that has frozen, as reported in WebKit's bug tracker.
  page.evaluate("window.frozenAudio=Module.SDL2.audioContext;const resume=frozenAudio.resume.bind(frozenAudio);Object.defineProperty(frozenAudio,'currentTime',{value:1.25,configurable:true});window.cycleResumes=0;frozenAudio.resume=()=>{cycleResumes++;delete frozenAudio.currentTime;return resume();};void 0;")
  try:
   page.wait_for_function("document.querySelector('#audio-status').textContent.includes('stopped responding')",timeout=7000)
  except Exception:
   print('Audio failure diagnostic',page.evaluate("({hidden:document.hidden,state:getAudio().state,time:getAudio().currentTime,observedTime,lastAudioProgress,lastAudioCallback,now:performance.now(),audioRecovering,audioGeneration,status:audioStatus.textContent})"),flush=True)
   raise
  page.locator('#canvas').click()
  page.wait_for_function("cycleResumes>0 && Module.SDL2.audioContext.state==='running'")
  page.wait_for_function("!document.querySelector('#audio-status').textContent",timeout=5000)
  assert project_hash()==before_restart
  print('Frozen-running clock recovery passed',flush=True)
  # A resume promise that never settles must not prevent an explicit restart.
  page.evaluate("window.stuckAudio=Module.SDL2.audioContext;stuckAudio.resume=()=>new Promise(()=>{});stuckAudio.suspend()")
  page.locator('#canvas').click()
  page.wait_for_function("document.querySelector('#audio-status').textContent.includes('Tap Restart audio')",timeout=5000)
  page.locator('#restart-audio').click()
  page.wait_for_function("Module.SDL2.audioContext!==stuckAudio && document.querySelector('#audio-status').textContent.includes('output restarted')",timeout=5000)
  assert project_hash()==before_restart
  print('Pending resume timeout and output restart preserved song bytes',flush=True)
  # Restart an output whose callback node has stopped, while its clock runs.
  page.evaluate("window.disconnectedAudio=Module.SDL2.audioContext;Module.SDL2.audio.scriptProcessorNode.disconnect()")
  try:
   page.wait_for_function("document.querySelector('#audio-status').textContent.includes('stopped responding')",timeout=7000)
  except Exception:
   print('Audio failure diagnostic',page.evaluate("({hidden:document.hidden,state:getAudio().state,time:getAudio().currentTime,observedTime,lastAudioProgress,lastAudioCallback,now:performance.now(),audioRecovering,audioGeneration,status:audioStatus.textContent})"),flush=True)
   raise
  page.locator('#restart-audio').click()
  page.wait_for_function("Module.SDL2.audioContext!==disconnectedAudio && document.querySelector('#audio-status').textContent.includes('output restarted')",timeout=5000)
  assert project_hash()==before_restart
  print('Missing audio callback detected and output restarted',flush=True)
  page.evaluate("window.closedAudio=Module.SDL2.audioContext;closedAudio.close()")
  page.locator('#restart-audio').click()
  page.wait_for_function("Module.SDL2.audioContext!==closedAudio && document.querySelector('#audio-status').textContent.includes('output restarted')",timeout=5000)
  assert project_hash()==before_restart
  print('Closed context restarted without losing the song',flush=True)

  page.evaluate("window.afterRestartPeak=0;const node=Module.SDL2.audio.scriptProcessorNode,callback=node.onaudioprocess;node.onaudioprocess=e=>{callback(e);for(const v of e.outputBuffer.getChannelData(0))afterRestartPeak=Math.max(afterRestartPeak,Math.abs(v));};void 0;")
  assert page.evaluate("Module.ccall('webLoadProject','number',['string'],['/projects/native-chip-audition.cct'])")==0
  page.locator('#canvas').click();key('Space')
  page.wait_for_function('afterRestartPeak>0',timeout=5000)
  # Reload must wait for durable storage and cancel on a storage error.
  page.evaluate("window.reloadMarker=1;window.realSyncfs=FS.syncfs;FS.syncfs=(populate,done)=>done(new Error('Injected storage failure'));void 0;")
  page.locator('#reload-app').click()
  page.wait_for_function("document.querySelector('#status').textContent.includes('Reload cancelled')")
  assert page.evaluate('reloadMarker')==1
  page.evaluate("FS.syncfs=realSyncfs;FS.writeFile('/user/reload-check.txt','kept')")
  before_reload=project_hash()
  with page.expect_navigation(wait_until='load'):
   page.locator('#reload-app').click()
  ready(page)
  assert page.evaluate("FS.readFile('/user/reload-check.txt',{encoding:'utf8'})")=='kept'
  assert project_hash()==before_reload
  print('Reload cancels on storage failure and preserves saved song/data on success',flush=True)
  print('JavaScript errors:',errors,flush=True);assert not errors
 finally:browser.close();server.shutdown()
