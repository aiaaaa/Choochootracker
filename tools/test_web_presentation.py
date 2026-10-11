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
  g=geometry(page);assert abs(g['scale']-round(g['scale']))<.01 and g['below'],g
  pixels=page.locator('#canvas').screenshot(scale='device')
  blocks=page.evaluate("""async ({png,scale})=>{const img=new Image();img.src='data:image/png;base64,'+png;await img.decode();const c=document.createElement('canvas');c.width=img.width;c.height=img.height;const ctx=c.getContext('2d');ctx.drawImage(img,0,0);const d=ctx.getImageData(0,0,c.width,c.height).data;let mismatches=0,total=0;for(let y=scale*2;y<c.height-scale*2;y+=scale)for(let x=scale*2;x<c.width-scale*2;x+=scale){total++;const i=(y*c.width+x)*4;let mismatch=false;for(let dy=0;dy<scale;dy++)for(let dx=0;dx<scale;dx++){const j=((y+dy)*c.width+x+dx)*4;for(let k=0;k<3;k++)if(d[i+k]!==d[j+k])mismatch=true;}if(mismatch)mismatches++;}return {mismatches,total,width:c.width,height:c.height};}""",{'png':base64.b64encode(pixels).decode(),'scale':round(g['scale'])})
  print('Uniform physical pixel blocks',blocks,flush=True);assert blocks['mismatches']==0,blocks
  print('Retina layout',g,flush=True);page.screenshot(path=str(root/'.tmp/theme-desktop.png'),full_page=True)
  page.set_viewport_size({'width':1100,'height':700});page.wait_for_timeout(250);g=geometry(page);assert abs(g['scale']-round(g['scale']))<.01 and g['below'],g
  print('Resized layout',g,flush=True)
  for width,height,dpr in [(390,844,3),(1440,900,1),(1024,768,1.25)]:
   c=browser.new_context(viewport={'width':width,'height':height},device_scale_factor=dpr,is_mobile=dpr==3,has_touch=dpr==3)
   t=c.new_page();t.goto(url);ready(t);g=geometry(t);assert abs(g['scale']-round(g['scale']))<.01 and g['below'],g
   print('Layout',width,height,dpr,g,flush=True)
   if dpr==3:
    t.locator('[data-button="4"]').tap();t.wait_for_timeout(500);t.screenshot(path=str(root/'.tmp/theme-phone.png'),full_page=True)
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
  print('JavaScript errors:',errors,flush=True);assert not errors
 finally:browser.close();server.shutdown()
