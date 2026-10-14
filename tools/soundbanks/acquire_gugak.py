"""Acquire publisher-linked EXS24 folders for private, noncommercial USER evaluation.

No sample content is committed or packaged. Dropbox constructs these ZIPs on
request, so their container checksum can change between acquisitions. Original
member checksums are recorded by inventory.py after acquisition.
"""
import argparse
from concurrent.futures import ThreadPoolExecutor, as_completed
import datetime
import hashlib
import json
from pathlib import Path
import re
import subprocess
from urllib.parse import unquote
import zipfile

REPO = Path(__file__).resolve().parents[2]


def sha256(path):
    h=hashlib.sha256()
    with path.open('rb') as f:
        while block:=f.read(1024*1024):h.update(block)
    return h.hexdigest()


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('data_root',type=Path)
    args=parser.parse_args();root=args.data_root.resolve()
    catalog=json.loads((REPO/'docs/soundbanks/gugak-sources.json').read_text())
    base=root/'Korean_Gugak';base.mkdir(parents=True,exist_ok=True)
    def download(item):
        result=dict(item);folder=base/item['family']/item['id'];folder.mkdir(parents=True,exist_ok=True)
        result['acquisition_date']=datetime.date.today().isoformat()
        try:
            existing=list(folder.glob('*.zip'))
            if len(existing)>1:raise ValueError('Multiple original archives: review manually')
            if existing:target=existing[0]
            else:
                partial=folder/'download.zip.part';headers=folder/'download-headers.local.txt'
                subprocess.run(['curl','--fail','--location','--retry','3','--max-time','1800','--silent','--show-error','--dump-header',str(headers),'--output',str(partial),item['download_url']],check=True)
                header=headers.read_text(errors='replace')
                names=re.findall(r"filename\*=UTF-8''([^\r\n;]+)",header,re.I)
                if names:name=unquote(names[-1])
                else:
                    names=re.findall(r'filename="([^"]+)"',header,re.I)
                    name=names[-1] if names else item['id']+' EXS24.zip'
                if Path(name).name!=name or not name.lower().endswith('.zip'):raise ValueError('Unsafe or unexpected download filename')
                with zipfile.ZipFile(partial) as archive:
                    if archive.testzip() is not None:raise ValueError('ZIP checksum failure')
                target=folder/name;partial.rename(target)
            with zipfile.ZipFile(target) as archive:
                if archive.testzip() is not None:raise ValueError('ZIP checksum failure')
                result['members']=len(archive.infolist())
                result['exs_definitions']=sum(x.filename.lower().endswith('.exs') for x in archive.infolist())
            result.update(status='acquired_archive_verified',filename=target.name,relative_path=target.relative_to(root).as_posix(),download_bytes=target.stat().st_size,sha256=sha256(target))
        except (OSError,ValueError,subprocess.CalledProcessError,zipfile.BadZipFile) as error:
            result.update(status='incomplete_or_unavailable',error=str(error))
        print(item['id'],result['status'],result.get('download_bytes',0),flush=True)
        return result
    result=dict(source=catalog['source'],license=catalog['license'],redistribution=catalog['redistribution'],artifacts=[])
    with ThreadPoolExecutor(max_workers=3) as pool:
        futures=[pool.submit(download,item) for item in catalog['instruments']]
        for future in as_completed(futures):
            result['artifacts'].append(future.result())
            (base/'acquisition.json').write_text(json.dumps(result,indent=2,ensure_ascii=False)+'\n')
    result['artifacts'].sort(key=lambda x:(x['family'],x['id']))
    (base/'acquisition.json').write_text(json.dumps(result,indent=2,ensure_ascii=False)+'\n')
    if any(a['status']!='acquired_archive_verified' for a in result['artifacts']):raise SystemExit(1)


if __name__=='__main__':main()
