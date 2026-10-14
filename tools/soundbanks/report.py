"""Summarize retained offline probe evidence; never infer success from acquisition."""
import argparse
import collections
import json
import statistics
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
RUNS = [
    ('VSCO_2_Community', 'vsco-instruments.jsonl', 'SFZ', True),
    ('VCSL', 'vcsl-instruments.jsonl', 'SFZ', True),
    ('Meta_Open_Sound', 'meta.jsonl', 'SFZ/Ogg', True),
    ('Meta_Open_Sound', 'meta-nylon-wav.jsonl', 'SFZ/WAV nylon', True),
    ('Meta_Open_Sound', 'meta-steel-wav.jsonl', 'SFZ/WAV steel', True),
    ('Meta_Open_Sound', 'meta-piano-wav.jsonl', 'SFZ/WAV piano', True),
    ('Stargate', 'stargate.jsonl', 'WAV/AIFF singles', False),
    ('FluidR3Mono', 'fluid-sf3.jsonl', 'SF3', True),
    ('GeneralUser_GS', 'generaluser.jsonl', 'SF2', True),
    ('Akai_S612_Modular', 'akai.jsonl', 'EXS/SFZ', True),
    ('Salamander', 'salamander.jsonl', 'SFZ/WAV', True),
    ('Korean_Gugak', 'gugak-legacy.jsonl', 'EXS/WAV legacy ZIPs', True),
    ('Korean_Gugak', 'gugak-latest.jsonl', 'EXS/WAV v1.3.1 original files', True),
]


def rows(path):
    return [json.loads(line) for line in path.read_text().splitlines()]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('data_root', type=Path)
    args = parser.parse_args()
    data = args.data_root.resolve()
    manifest_path = REPO / 'soundbank_manifest.json'
    manifest = json.loads(manifest_path.read_text())
    by_id = {c['id']: c for c in manifest['collections']}
    report = dict(date='2026-10-14', method='256 MiB PCM cache per preset; parse, prepare, render 1024 frames at 48 kHz. No fidelity/listening assertion.', results=[], runs=[])
    for cid, filename, variant, mappings in RUNS:
        records = rows(data / 'results' / filename)
        if mappings:
            records = [r for r in records if Path(r['source']).suffix.lower() in ('.sfz', '.exs', '.sf2', '.sf3')]
        errors = collections.Counter()
        warnings = collections.Counter()
        for r in records:
            r = dict(r)
            r['source'] = r['source'].split('soundbank-data/', 1)[-1]
            r['collection'] = cid
            r['variant'] = variant
            r['warnings'] = sorted(set(r.get('warnings', [])))
            for warning in r['warnings']: warnings[warning] += 1
            if r.get('error'):
                error = r['error']
                if 'cache limit' in error or 'cache budget' in error: reason = 'PCM cache budget'
                elif 'Missing ZIP sample' in error or 'Cannot find sample' in error: reason = 'Missing source sample'
                else: reason = error.split(':', 1)[0]
                errors[reason] += 1
            report['results'].append(r)
        successful = [r for r in records if r.get('loaded')]
        timings = [r['prepare_render_ms'] for r in successful if r.get('prepare_render_ms')]
        report['runs'].append(dict(collection=cid, variant=variant, evidence='results/'+filename,
            banks=len({r['source'] for r in records}), outcomes=len(records), loaded=len(successful), failed=len(records)-len(successful),
            presets_with_warnings=sum(bool(r.get('warnings')) for r in records), errors=dict(errors), unsupported_warnings=dict(warnings),
            load_ms_median=statistics.median(timings) if timings else None, load_ms_max=max(timings) if timings else None))
    results = report['results']
    report['totals'] = dict(covered_banks=sum(r['banks'] for r in report['runs']), preset_outcomes=len(results),
        loaded=sum(bool(r.get('loaded')) for r in results), failed=sum(not r.get('loaded') for r in results),
        presets_with_warnings=sum(bool(r.get('warnings')) for r in results),
        unique_warning_messages=len({w for r in results for w in r.get('warnings', [])}),
        detected_missing_asset_references=len({r['error'] for r in results if 'Missing ZIP sample' in r.get('error', '')}))
    report['metadata_index'] = dict(first=rows(data/'results/index-first.jsonl')[0], cached=rows(data/'results/index-cached.jsonl')[0],
        note='Pre-Gugak external data-directory scan includes alternate distributions, raw single samples, extracted FluidR3Mono and Salamander. Gugak playback is reported separately. Totals are not unique musical instruments.')
    report['supplemental'] = {f: rows(data/'results'/f) for f in ('vcsl-large-1024.jsonl','meta-piano-1024.jsonl','salamander-1024.jsonl','r36h-fluid-sf3.jsonl','gugak-latest-64.jsonl')}
    for c in manifest['collections']:
        selected = [r for r in report['runs'] if r['collection']==c['id']]
        c['compatibility'] = dict(report='docs/soundbanks/compatibility.json', runs=selected,
            status='offline_tested_with_documented_limits' if selected else 'not_tested',
            instrument_inventory='docs/soundbanks/compatibility.json#results')
    artifacts = [a for c in manifest['collections'] for a in c.get('artifacts', [])]
    manifest['totals'] = dict(acquired_collections=sum(bool(c.get('artifacts')) for c in manifest['collections']),
        artifacts=len(artifacts), archive_bytes=sum(a.get('download_bytes',0) for a in artifacts),
        unpacked_artifact_bytes=sum(a.get('unpacked_bytes') or 0 for a in artifacts),
        factory_approved_collections=0, license_candidates_pending_review=6, provenance_review_collections=2, private_restricted_collections=1, unverified_unacquired_collections=0)
    (REPO/'docs/soundbanks/compatibility.json').write_text(json.dumps(report, indent=2, ensure_ascii=False)+'\n')
    manifest_path.write_text(json.dumps(manifest,indent=2,ensure_ascii=False)+'\n')
    text = ['# Library compatibility report', '', 'Measured 2026-10-14 on the development Mac, except the separately labeled R36H results.', '',
        '“Loaded” means complete preset preparation and a 1024-frame finite-render smoke test at 48 kHz, using a fresh 256 MiB PCM cache. It does **not** mean full sampler fidelity, every articulation tested, or suitability for the 64 MiB handheld default. Warnings remain partial compatibility. Counts include alternate distributions; raw collection assets are not all separate multisample instruments.', '',
        '| Collection / distribution | Format | Preset outcomes | Loaded | Failed | Failure reasons | License | Collection archive bytes¹ |',
        '| --- | --- | ---: | ---: | ---: | --- | --- | ---: |']
    for r in report['runs']:
        c=by_id[r['collection']]
        footprint=sum(a.get('download_bytes',0) for a in c.get('artifacts',[]))
        errors='; '.join(f'{k}: {v}' for k,v in r['errors'].items()) or 'None in this smoke test'
        text.append(f"| {c['name']} / {r['variant']} | {r['variant']} | {r['outcomes']} | {r['loaded']} | {r['failed']} | {errors} | {c['license']} | {footprint:,} |")
    text += ['',
        '¹ Collection totals repeat for alternate distributions in this table; do not sum the table’s size column.', '', '## Totals and limits', '']
    for k,v in report['totals'].items():text.append(f'- {k.replace("_"," ")}: **{v:,}**.')
    total=manifest['totals']
    text += ['', f"Acquired **{total['acquired_collections']} collections / {total['artifacts']} complete original artifacts**, **{total['archive_bytes']:,} archive bytes** ({total['archive_bytes']/2**30:.2f} GiB), with **{total['unpacked_artifact_bytes']:,} member bytes** across distributions. Akai's current repository is complete as downloaded but does not contain the advertised original collection's full audio. No Factory content is packaged.", '',
        'Six collections are license candidates pending compatibility/provenance review; Stargate and GeneralUser require additional provenance review. Gugak is acquired for private USER evaluation under its publisher notice; the separate National Gugak Center KOGL Type 1 notice has not been verified as covering these EXS24 files. Zero collections are Factory-approved. These are review statuses, not blanket redistribution permission.', '',
        'Missing-asset counts are unique references encountered before preparation stopped at the first failure per preset, not an exhaustive count of every missing file. Five malformed SFZ files have no discoverable preset and are counted as failed instrument outcomes. Full per-member hashes and instrument names remain in the manifest/external inventories and [machine-readable report](compatibility.json).', '',
        f"Metadata scanning found **{report['metadata_index']['first']['banks']:,} banks/assets and {report['metadata_index']['first']['presets']:,} preset entries**, with {report['metadata_index']['first']['bank_errors']} parse errors. First scan: **{report['metadata_index']['first']['index_ms']:.2f} ms**; cached scan: **{report['metadata_index']['cached']['index_ms']:.2f} ms**. This pre-Gugak scan includes raw samples and duplicate distributions, and decodes no sample audio. Gugak playback results above are additional. It is distinct from the smaller mapped-instrument playback suite above.", '',
        '## Recurring compatibility gaps', '',
        '- VCSL: three pianos exceed 256 MiB. With 1024 MiB configured, Kawai, Steinway B and Knight all prepare (455,933,860 / 893,137,864 / 274,112,236 PCM bytes respectively). They are not suitable for the 64 MiB handheld default. Gong 2 references unsupported MP3. CC-triggered pedal samples, pedal conditions and release-decay opcodes remain unsupported and reported.',
        '- Meta: the repository piano uses Ogg Opus, which the Vorbis decoder explicitly rejects. The complete WAV piano release prepares at 1024 MiB (590,935,736 PCM bytes). The guitars load, but their full selected articulations also exceed the 64 MiB default. No source conversion was performed.',
        '- Salamander: metadata and 637 regions parse; the full preset exceeds even the 1024 MiB PCM budget. This is acquired and indexed, not successfully loaded. Streaming or a disk-backed sample strategy would be the next substantial memory improvement.',
        '- Akai: 102 EXS definitions and 18 SFZ definitions are present; only two EXS instruments have all required samples and prepare. Most references are absent from the publisher repository; five SFZ definitions are malformed. EXS group/instrument synthesis parameters warn. This is limited real-world EXS evidence, not broad EXS compatibility.',
        '- Gugak: all 23 publisher EXS24 folders acquired, including older versions. Latest v1.3.1 tests load 41/41 presets (10 with unsupported-parameter warnings). Those ZIPs contain Mac disk images: an optional macOS acquisition helper copied unchanged Data-tree files from read-only mounts without running installers. The application does not read DMG or nested ZIP containers. Legacy ZIP testing covers 89 presets: all 89 prepare after preserving original EXS directory suffixes to disambiguate duplicate Ajaeng bow-sample filenames. Source audio and mappings are unchanged. Original archives, manuals and attribution remain in the private sideload bundle. EXS presets do not reproduce separate Logic channel strips, effects or plug-in settings. At the 64 MiB handheld cache budget, 40/41 current presets prepare on the Mac; the combined Geomungo preset needs 79,795,564 PCM bytes. This budget test is not an R36H playback measurement.',
        '- Stargate: four samples have out-of-range loop metadata, two advertise unsupported loop modes and one contains a partial PCM frame. These fail explicitly rather than silently repairing source audio.',
        '- SF2/SF3: GeneralUser and FluidR3Mono prepare completely, but unsupported custom modulators/generators can alter timbre. SF3 testing covers mono Vorbis sample streams and two observed sample-end conventions.',
        '- SFZ articulation switches, sustain pedal, dynamic envelopes, rt_decay and additional filter modes are the highest-value remaining parser/renderer improvements. Large-preset streaming is needed for the largest pianos; increasing the handheld cache alone is not a solution.', '',
        '## Reproduce', '',
        'Run `tools/soundbanks/acquire.py`, then `inventory.py`, and the probe commands in the [guide](README.md). Retain JSONL under the external data root’s `results/` directory. Run `python3 tools/soundbanks/report.py /external/soundbank-data` after those tests to regenerate this report. The report script requires actual test records and never marks an untested acquisition as loaded.', '',
        'See [validation and performance](validation.md) for platform build results and isolated hardware measurements.']
    (REPO/'docs/soundbanks/compatibility.md').write_text('\n'.join(text)+'\n')


if __name__ == '__main__':main()
