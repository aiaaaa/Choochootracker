"""Build original, editable SID sounds into an explicitly chosen USER folder.

No source recordings, ripped songs, imported programs or factory duplicates.
The recipes and generator are project-authored under the repository MIT license.
"""
import argparse
import hashlib
import json
from pathlib import Path

FIELDS = ('reserved wave attack decay sustain release pulse cutoff resonance filter '
          'ring sync partner clock gate fixed target sweep exponential pulse_depth '
          'pulse_rate vibrato vibrato_rate cutoff_target arp').split()
DEFAULT = dict(zip(FIELDS, [0, 4, 0, 6, 10, 4, 2048, 1200, 3, 0, 0, 0, 2,
                            50, 0, 0, 0, 0, 1, 0, 0, 0, 0, 65535, 0]))
# Each category has a starting voice; every named sound has its own recipe.
RECIPES = {
 'Bass': (dict(filter=1, cutoff=600, sustain=9, release=2), [
  ('Velvet Sub', dict(wave=1, cutoff=1100, sustain=12, decay=3)),
  ('Rubber Bass', dict(pulse=900, cutoff=1100, cutoff_target=180, decay=9, resonance=8)),
  ('Acid Needle', dict(wave=2, cutoff=950, cutoff_target=130, resonance=13, sustain=5)),
  ('Hollow Square', dict(pulse=2048, cutoff=1400, sustain=11)),
  ('Narrow Drive', dict(pulse=350, cutoff=850, resonance=5, pulse_depth=160, pulse_rate=9)),
  ('Saw Anchor', dict(wave=2, cutoff=380, sustain=13, attack=1)),
  ('Sync Engine', dict(wave=2, sync=1, partner=2, cutoff=650, resonance=4)),
  ('Muted Pluck', dict(wave=4, pulse=1300, decay=8, sustain=0, cutoff=500, release=3)),
 ]),
 'Leads': (dict(sustain=12, release=5), [
  ('Clear Triangle', dict(wave=1, vibrato=9, vibrato_rate=45)),
  ('Bright Saw', dict(wave=2, filter=1, cutoff=1700, resonance=2)),
  ('Singing Pulse', dict(pulse=1200, pulse_depth=500, pulse_rate=5, vibrato=12, vibrato_rate=50)),
  ('Pin Lead', dict(pulse=180, filter=1, cutoff=1450, resonance=3)),
  ('Octave Sync', dict(wave=2, sync=1, partner=2, filter=1, cutoff=1400)),
  ('Fifth Ring', dict(wave=1, ring=1, partner=3, filter=1, cutoff=1600)),
  ('Soft Whistle', dict(wave=1, attack=3, release=7, vibrato=18, vibrato_rate=36)),
  ('Biting Reed', dict(pulse=600, filter=2, cutoff=900, resonance=7, vibrato=6, vibrato_rate=48)),
 ]),
 'Plucks': (dict(decay=9, sustain=0, release=4), [
  ('Copper String', dict(wave=2, filter=1, cutoff=1200, cutoff_target=300)),
  ('Glass Pin', dict(wave=1, ring=1, partner=5, decay=10)),
  ('Short Square', dict(pulse=2048, decay=5)),
  ('Nylon Chip', dict(pulse=1800, filter=1, cutoff=700, decay=10)),
  ('Wire Pick', dict(pulse=300, filter=1, cutoff=1500, resonance=4)),
  ('Wood Block', dict(wave=1, fixed=5000, target=2600, sweep=3, decay=5)),
  ('Resonant Drop', dict(wave=2, filter=1, cutoff=1700, cutoff_target=160, resonance=12, decay=11)),
  ('Tiny Kalimba', dict(wave=1, ring=1, partner=2, decay=8, filter=1, cutoff=1100)),
 ]),
 'Keys': (dict(decay=11, sustain=3, release=8), [
  ('Triangle Tines', dict(wave=1, ring=1, partner=3)),
  ('Warm Chip Keys', dict(pulse=1800, filter=1, cutoff=700)),
  ('Toy Organ', dict(pulse=2048, sustain=13, attack=1, release=2)),
  ('Soft Clav', dict(pulse=750, filter=1, cutoff=1050, decay=7, sustain=0)),
  ('Digital Celeste', dict(wave=1, ring=1, partner=6, decay=12, sustain=0)),
  ('Reed Keys', dict(pulse=900, filter=2, cutoff=650, resonance=3, sustain=8)),
  ('Low Electric', dict(wave=1, decay=12, sustain=4, vibrato=4, vibrato_rate=42)),
  ('Bright Hammer', dict(wave=2, filter=1, cutoff=1450, cutoff_target=450, decay=9, sustain=1)),
 ]),
 'Pads': (dict(attack=10, decay=9, sustain=11, release=10, filter=1), [
  ('Slow Amber', dict(wave=2, cutoff=650, cutoff_target=1200)),
  ('Pulse Cloud', dict(pulse=1600, pulse_depth=650, pulse_rate=3, cutoff=1000)),
  ('Hollow Air', dict(pulse=2048, cutoff=430, resonance=6)),
  ('Silver Mist', dict(wave=1, ring=1, partner=5, cutoff=850)),
  ('Dusk Triangle', dict(wave=1, cutoff=1300, vibrato=7, vibrato_rate=22)),
  ('Opening Brass', dict(wave=2, attack=7, cutoff=250, cutoff_target=1450, resonance=4)),
  ('Narrow Horizon', dict(pulse=400, pulse_depth=220, pulse_rate=2, cutoff=750, vibrato=5, vibrato_rate=31)),
  ('Frozen Fifth', dict(wave=1, ring=1, partner=3, cutoff=1350, attack=11, release=11)),
 ]),
 'Arps': (dict(decay=7, sustain=10, release=4, clock=36), [
  ('Major Spark', dict(wave=4, pulse=1100, arp=1)),
  ('Minor Lantern', dict(wave=2, filter=1, cutoff=1200, arp=2)),
  ('Open Fifth', dict(wave=1, arp=3, clock=28)),
  ('Suspended Glass', dict(wave=1, ring=1, partner=2, arp=4, clock=42)),
  ('Major Ribbon', dict(pulse=1700, pulse_depth=500, pulse_rate=4, arp=1, clock=24)),
  ('Minor Motor', dict(pulse=500, filter=1, cutoff=900, resonance=6, arp=2, clock=60)),
  ('Fifth Beacon', dict(wave=2, filter=2, cutoff=1000, arp=3, clock=48)),
  ('Sus Echo', dict(pulse=2200, arp=4, clock=18, decay=10, sustain=5, release=9)),
 ]),
 'Drums': (dict(decay=6, sustain=0, release=2, gate=20), [
  ('Deep Kick', dict(wave=1, fixed=9000, target=650, sweep=7, decay=9)),
  ('Hard Kick', dict(wave=1, fixed=18000, target=900, sweep=4, decay=7)),
  ('Low Tom', dict(wave=1, fixed=5000, target=1800, sweep=9, decay=9)),
  ('High Tom', dict(wave=1, fixed=10000, target=3400, sweep=6, decay=7)),
  ('Noise Snare', dict(wave=8, fixed=18000, filter=2, cutoff=1200, resonance=3, decay=7)),
  ('Closed Hat', dict(wave=8, fixed=50000, filter=4, cutoff=1300, decay=3, gate=4)),
  ('Open Hat', dict(wave=8, fixed=46000, filter=4, cutoff=1200, decay=9, gate=25)),
  ('Metal Clang', dict(wave=1, ring=1, partner=7, fixed=14000, decay=10, gate=30)),
 ]),
 'Effects': (dict(sustain=10, release=6, gate=60), [
  ('Laser Tick', dict(wave=2, fixed=40000, target=700, sweep=15, decay=7, sustain=0)),
  ('Bubble Rise', dict(wave=4, pulse=700, fixed=1200, target=18000, sweep=35, filter=1, cutoff=1100)),
  ('Noise Fall', dict(wave=8, fixed=32000, target=2500, sweep=50, filter=1, cutoff=1500, cutoff_target=80)),
  ('Ring Alarm', dict(wave=1, ring=1, partner=5, fixed=7000, vibrato=2000, vibrato_rate=60)),
  ('Pulse Radar', dict(pulse=500, fixed=9000, pulse_depth=400, pulse_rate=35, gate=25)),
  ('Filter Rocket', dict(wave=2, fixed=3000, target=26000, sweep=65, filter=1, cutoff=200, cutoff_target=1800, resonance=12, gate=75)),
  ('Arcade Fall', dict(wave=4, pulse=1800, fixed=24000, target=1200, sweep=45, arp=2, gate=50)),
  ('Wind Tunnel', dict(wave=8, fixed=22000, filter=2, cutoff=300, cutoff_target=1500, resonance=9, attack=8, gate=90)),
 ]),
}

def build(root):
    records = []
    signatures = set()
    for category, (base, sounds) in RECIPES.items():
        for name, changes in sounds:
            values = DEFAULT | base | changes
            parameters = ','.join(str(values[k]) for k in FIELDS)
            assert parameters not in signatures, name
            signatures.add(parameters)
            text = ('# ChipNomad Instrument 7.0\n\n### Instrument 0\n\n'
                    f'- Name: {name[:15]}\n- Type: 27\n- Table speed: 1\n'
                    '- Volume: 255\n- Transpose: 1\n- Modulation:\n')
            text += ''.join(f'- Mod{m}: 0,0,0,0,0,0,0,0\n' for m in range(1,5))
            text += (f'- Chip data:\n- SID identity: 1,602\n- SID name: {name}\n'
                     f'- SID parameters: {parameters}\n\n### Table 0 (Retrig: Inst)\n\n```\n')
            text += '~ 00 -- --- 00 --- 00 --- 00 --- 00\n' * 16 + '```\n'
            path = root / f'Studio {category}' / f'{name}.cni'
            path.parent.mkdir(parents=True, exist_ok=True)
            if path.exists() and path.read_text() != text:
                raise FileExistsError(f'Refusing to replace edited sound: {path}')
            path.write_text(text)
            records.append(dict(category=category, name=name, file=str(path.relative_to(root)),
                                sha256=hashlib.sha256(path.read_bytes()).hexdigest()))
    return records

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True, help='Destination USER/sid directory')
    parser.add_argument('--manifest', type=Path, required=True, help='Manifest outside instrument folders')
    args = parser.parse_args()
    records = build(args.output)
    args.manifest.parent.mkdir(parents=True, exist_ok=True)
    args.manifest.write_text(json.dumps(dict(collection='ChooChoo SID Studio', license='MIT',
        source='Original parameter recipes; no song extraction', sounds=records), indent=2)+'\n')
    print(f'Built {len(records)} editable sounds in {len(RECIPES)} collections')
