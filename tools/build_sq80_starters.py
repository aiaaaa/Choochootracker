#!/usr/bin/env python3
"""Build original SQ80/ESQ teaching presets; contains no sampled waveform data."""
from pathlib import Path
import zipfile
ROOT = Path(__file__).resolve().parents[1]
def patch(name, wave=0, cutoff=100, attack=0, release=24):
    b=bytearray(102); b[:6]=name.encode().ljust(6,b' ')[:6]
    for e in range(4):
        n=6+10*e;b[n:n+7]=bytes([126,90,80,attack,24,16,release])
    for l in range(3):
        n=46+4*l;b[n:n+4]=bytes([24,192,192,128])
    for o in range(3):
        n=58+10*o;b[n]=36;b[n+2]=255;b[n+5]=wave;b[n+6]=100|(128 if o==0 else 0);b[n+7]=255
    b[88]=100;b[89]=cutoff;b[91]=255;b[94]=128;b[95]=128;b[100]=143
    return b

def sysex(b):
    return bytes([240,15,2,0,1])+bytes(v for x in b for v in (x&15,x>>4))+bytes([247])

def build():
    sounds=[]
    for name,wave in [('SAW',0),('SQUARE',3),('SINE',2),('BELL',1)]:sounds.append((name,patch(name,wave)))
    b=patch('WHEEL',0,40);b[91]=251;b[92]=48;sounds.append(('Wheel Filter',b))
    b=patch('PEDAL',3,35);b[91]=252;b[92]=50;sounds.append(('Pedal Filter',b))
    b=patch('PRESS',0,35,30,35);b[91]=254;b[92]=55;sounds.append(('Pressure Pad',b))
    b=patch('BREATH',2,90);b[64]=128;b[65]=253;b[66]=126;sounds.append(('External Breath',b))
    b=patch('GLIDE',0,75);b[93]=128;b[95]=128|24;sounds.append(('Glide Lead',b))
    b=patch('VIBRO',3,90);b[60]=240;b[61]=10;b[47]=128;b[48]=192|40;sounds.append(('Wheel Vibrato',b)) # calibrated source 11
    b=patch('PLUCK',0,95,0,10);b[37]=0;b[38]=0;b[40]=22;b[41]=0;sounds.append(('Pluck',b))
    b=patch('SOFT',2,65,35,40);sounds.append(('Soft Pad',b))
    dest=ROOT/'tracker/packaging/common/instruments/FACTORY/sq80esq/701-hybrid-starters.zip'
    notice='Original ChooChooTracker SQ80/ESQ starter presets, created for Rev2.\nThese parameter programs are provided under the repository license.\nNo downloaded preset bytes or waveform samples are used. External wave banks are required.\n'
    with zipfile.ZipFile(dest,'w',zipfile.ZIP_DEFLATED) as z:
        for name,b in sounds:
            info=zipfile.ZipInfo('Hybrid Starters/'+name+'.syx',(2026,10,13,0,0,0));info.compress_type=zipfile.ZIP_DEFLATED;z.writestr(info,sysex(b))
        z.writestr(zipfile.ZipInfo('Hybrid Starters/README.txt',(2026,10,13,0,0,0)),notice)
    print(dest, len(sounds),'presets')
if __name__=='__main__':build()
