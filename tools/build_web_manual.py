"""Build the static web manual from docs/USER_MANUAL.md."""
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / ".tmp" / "manual-web-build"))
import markdown


ROOT = Path(__file__).resolve().parents[1]
source = (ROOT / "docs" / "USER_MANUAL.md").read_text(encoding="utf-8")
body = markdown.markdown(source, extensions=["tables", "fenced_code", "sane_lists"])
slides = re.split(r"(?=<h2(?: |>))", body)
titles = [re.sub(r"<[^>]+>", "", slide.split("</h2>", 1)[0]) if "<h2" in slide else "Cover" for slide in slides]
slides = "".join(f'<section class="slide">{slide}</section>' for slide in slides)
toc = "".join(f'<button data-slide="{i}">{title}</button>' for i, title in enumerate(titles))

style = """
html,body{margin:0;height:100%;overflow:hidden;background:#050c1f;color:#d7d1c0;font:16px system-ui,sans-serif}#deck{height:100%;display:grid;place-items:center}.slide{display:none;width:min(1000px,78vw);height:min(88vh,760px);overflow:auto;padding:3rem 4rem;box-sizing:border-box;background:#0b1930;border:1px solid #245072;box-shadow:0 0 35px #020611}.slide.active{display:block}.slide h1{margin:8vh 0 1rem;text-align:center;color:#ffd665;font-size:clamp(2rem,5vw,4rem)}.slide h1+p{text-align:center;color:#9ab1c4;font-size:1.2rem}.slide h2{color:#ffd665;font-size:clamp(1.5rem,3vw,2.3rem);border-bottom:1px solid #52799b;padding-bottom:.35rem}.slide h3{color:#65d8ff}.slide h4{color:#ff9e8a}.slide p,.slide li{line-height:1.5}.slide table{border-collapse:collapse;width:100%;font-size:.9rem}.slide th,.slide td{border:1px solid #52799b;padding:.4rem .55rem;text-align:left;vertical-align:top}.slide th{background:#10284a;color:#ffd665}.slide code{background:#10284a;color:#9fe8ff;padding:.1rem .25rem}.slide pre{background:#050c1f;padding:1rem;overflow:auto}a{color:#65d8ff}#toc{position:fixed;left:1rem;top:1rem;bottom:4rem;width:16vw;min-width:150px;overflow:auto}#toc button{display:block;width:100%;margin:.2rem 0;text-align:left;font-size:.76rem;background:#10284a;color:#9ab1c4}#toc button.active{background:#ffd665;color:#050c1f}#nav{position:fixed;bottom:1rem;left:50%;transform:translateX(-50%);display:flex;gap:.6rem;align-items:center}button{background:#ffd665;color:#050c1f;border:0;border-radius:.3rem;padding:.45rem .8rem;font-weight:700;cursor:pointer}#count{color:#9ab1c4;min-width:4rem;text-align:center}@media(max-width:700px){#toc{z-index:2;inset:.5rem .5rem auto;width:auto;min-width:0;display:flex;gap:.3rem;overflow-x:auto;white-space:nowrap;scrollbar-width:none}#toc button{display:inline-block;width:auto;flex:0 0 auto;margin:0;font-size:.7rem;padding:.4rem .55rem}#deck{display:block;padding-top:3.25rem;box-sizing:border-box}.slide{width:auto;max-width:none;height:calc(100dvh - 7rem);margin:0 .5rem;padding:1rem;font-size:14px}.slide h1{margin:4vh 0 1rem;overflow-wrap:anywhere}.slide table{display:block;overflow-x:auto}#nav{z-index:2;bottom:.5rem}}
"""
script = """const slides=[...document.querySelectorAll('.slide')],buttons=[...document.querySelectorAll('#toc button')],count=document.querySelector('#count');let n=0;function show(i){n=(i+slides.length)%slides.length;slides.forEach((s,j)=>s.classList.toggle('active',j===n));buttons.forEach((b,j)=>b.classList.toggle('active',j===n));count.textContent=(n+1)+' / '+slides.length}buttons.forEach((b,i)=>b.onclick=()=>show(i));document.querySelector('#prev').onclick=()=>show(n-1);document.querySelector('#next').onclick=()=>show(n+1);document.onkeydown=e=>{if(['ArrowRight',' ','PageDown'].includes(e.key))show(n+1);if(['ArrowLeft','PageUp'].includes(e.key))show(n-1);if(e.key==='Home')show(0);if(e.key==='End')show(slides.length-1)};show(0)"""
html = f'<!doctype html><html lang="en"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>ChooChooTracker User Manual</title><style>{style}</style></head><body><nav id="toc" aria-label="Contents">{toc}</nav><main id="deck">{slides}</main><nav id="nav"><button id="prev" aria-label="Previous slide">◀</button><span id="count"></span><button id="next" aria-label="Next slide">▶</button></nav><script>{script}</script></body></html>'
(ROOT / "web" / "dist" / "manual.html").write_text(html, encoding="utf-8")
