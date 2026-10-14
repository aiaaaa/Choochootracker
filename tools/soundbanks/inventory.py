"""Inventory original downloaded archives without altering their contents.

Bulk member inventories stay beside the external archives. Only summaries,
attribution documents and compatibility results are written to the repository.
Run: python3 tools/soundbanks/inventory.py /path/to/soundbank-data
"""
import argparse
from contextlib import nullcontext
import tempfile
import collections
import gzip
import hashlib
import json
from pathlib import Path
import tarfile
import zipfile

REPO = Path(__file__).resolve().parents[2]
DATE = "2026-10-14"
CANDIDATES = [
    dict(id="VSCO_2_Community", name="VSCO 2 Community Edition", publisher="Versilian Studios / Sam Gossner", source="https://github.com/sgossner/VSCO-2-CE", license="CC0-1.0", attribution="Not required by CC0; retain Versilian Studios and contributor credits.", provenance="Original publisher recordings; original README and per-folder notes retained.", rank=1, artifacts=[
        ("VSCO-2-CE-440300901dfe9275fd84e0b7763af1f8443ae62e.zip", "440300901dfe9275fd84e0b7763af1f8443ae62e", "raw"),
        ("VSCO-2-CE-SFZ-6dd651d55dde97fd4028699be9d4481f26917891.zip", "6dd651d55dde97fd4028699be9d4481f26917891", "sfz")]),
    dict(id="VCSL", name="Versilian Community Sample Library", publisher="Versilian Studios / Sam Gossner and contributors", source="https://github.com/sgossner/VCSL", license="CC0-1.0", attribution="Not required by CC0; retain contributor credits and original documentation.", provenance="Original publisher community recordings; main-branch license retained alongside SFZ-branch archive.", rank=2, artifacts=[
        ("VCSL-c1ea7bcc3c7309650ab0da9d15c9cd1fbc4a4c7e.zip", "c1ea7bcc3c7309650ab0da9d15c9cd1fbc4a4c7e", "raw"),
        ("VCSL-SFZ-dfcf4a4918771eee884b96ad4493de82ef84daf6.zip", "dfcf4a4918771eee884b96ad4493de82ef84daf6", "sfz")]),
    dict(id="Meta_Open_Sound", name="Meta Open Sound", publisher="Meta Creative Audio", source="https://github.com/facebookincubator/Meta-open-sound", license="CC0-1.0", attribution="Not required by CC0; retain Meta Creative Audio attribution and LICENSE.", provenance="Original Meta recording project; original Ogg repository and all three WAV instrument releases acquired.", rank=3, artifacts=[
        ("Meta-open-sound-e2ff154845994fe4135becc4d5b857c19f172784.zip", "e2ff154845994fe4135becc4d5b857c19f172784", "ogg"),
        *[(f"meta-open-sound-meta-{n}-wav.zip", f"meta-{n}-v1.0.0", "wav") for n in ("piano", "nylon-guitar", "steel-guitar")]]),
    dict(id="Stargate", name="Stargate Sample Pack", publisher="Stargate DAW and credited contributors", source="https://github.com/stargatedaw/stargate-sample-pack", license="CC0-1.0", attribution="Retain the original source-specific README and attribution records, including Freesound authors.", provenance="Compilation with contributor/source directories and CC0 license; per-asset provenance needs review before a public Factory selection.", rank=4, artifacts=[("stargate-sample-pack-dbfd6ec52d4ed53b60bdbea5fc6adf295127c027.zip", "dbfd6ec52d4ed53b60bdbea5fc6adf295127c027", "raw")]),
    dict(id="FluidR3Mono", name="FluidR3Mono 2.315", publisher="Frank Wen; Michael Cowgill; Ethan Winer; Michael Schorsch", source="https://packages.debian.org/sid/fluidr3mono-gm-soundfont", license="MIT", attribution="Preserve all copyright acknowledgements and the complete FluidR3Mono_License.md; see original license.", provenance="Debian original source distribution and packaged SF3 derived from FluidR3Mono. Original license and Debian copyright preserved.", rank=5, artifacts=[("fluidr3mono-gm-soundfont_2.315.orig.tar.xz", "2.315", "sf2"), ("fluidr3mono-gm-soundfont_2.315-7_all.deb", "2.315-7", "sf3")]),
    dict(id="Salamander", name="Salamander Grand Piano V3", publisher="Alexander Holm", source="https://freepats.zenvoid.org/Piano/acoustic-grand-piano.html", license="CC-BY-3.0", attribution="Credit Alexander Holm, link CC BY 3.0 and preserve README and license. Acquired recordings and mappings are unchanged.", provenance="Original piano recordings distributed by FreePats; 48 kHz 24-bit complete distribution.", rank=6, artifacts=[("SalamanderGrandPianoV3+20161209_48khz24bit.tar.xz", "V3+20161209", "sfz_wav")]),
    dict(id="GeneralUser_GS", name="GeneralUser GS", publisher="S. Christian Collins and credited contributors", source="https://github.com/mrbumpy409/GeneralUser-GS", license="LicenseRef-GeneralUser-GS", attribution="Preserve original LICENSE.txt, README and all acknowledgements; consult custom license before redistribution.", provenance="Publisher explicitly cannot confirm every historical sample's origin. Do not approve for Factory redistribution merely because the bank is freely downloadable.", rank=6, artifacts=[("GeneralUser-GS-684543d5e5efaef08d02be50dcda8d552478fa60.zip", "684543d5e5efaef08d02be50dcda8d552478fa60", "sf2")]),
    dict(id="Akai_S612_Modular", name="Akai S612 Modular", publisher="Modular Samples / publicsamples", source="https://github.com/publicsamples/Akai-S612-Modular", license="LicenseRef-PublicDomain-Unlicense-Content", attribution="Publisher public-domain dedication (Unlicense wording adapted to content); retain original LICENSE and publisher identity.", provenance="Publisher describes own modular, Machinedrum and Jomox recordings through Akai S612. Current repository is only ~32 MB versus advertised ~2 GB; many original sample references are absent.", rank=7, artifacts=[("Akai-S612-Modular-4fe40c94ca973ce10bde35fbcca420d77b4a20ff.zip", "4fe40c94ca973ce10bde35fbcca420d77b4a20ff", "exs_sfz")]),
]

def digest(stream):
    h = hashlib.sha256()
    while block := stream.read(1024 * 1024):
        h.update(block)
    return h.hexdigest()

def url_for(c, filename, version):
    if c["id"] == "Salamander":
        return "https://freepats.zenvoid.org/Piano/SalamanderGrandPiano/" + filename
    if c["id"] == "FluidR3Mono":
        return "https://ftp.debian.org/debian/pool/main/f/fluidr3mono-gm-soundfont/" + filename
    if c["id"] == "Meta_Open_Sound" and filename.startswith("meta-open-sound-meta-"):
        return c["source"] + "/releases/download/" + version + "/" + filename
    return c["source"].replace("https://github.com/", "https://codeload.github.com/") + "/zip/" + version

def inventory(c, path, data_root):
    formats, docs = collections.Counter(), []
    output = data_root / "inventories" / (path.name + ".jsonl.gz")
    output.parent.mkdir(exist_ok=True)
    temporary = None
    if path.suffix == ".deb":
        with path.open("rb") as package:
            if package.read(8) != b"!<arch>\n":
                raise ValueError("Invalid Debian ar archive")
            while header := package.read(60):
                if len(header) != 60 or header[58:] != b"`\n":
                    raise ValueError("Truncated Debian ar header")
                size = int(header[48:58])
                name = header[:16].decode("ascii").strip().rstrip("/")
                if name.startswith("data.tar"):
                    temporary = tempfile.TemporaryFile()
                    left = size
                    while left:
                        block = package.read(min(left, 1024*1024))
                        if not block: raise ValueError("Truncated Debian data archive")
                        temporary.write(block); left -= len(block)
                    temporary.seek(0)
                    break
                package.seek(size + size % 2, 1)
            if temporary is None: raise ValueError("Debian data archive missing")
    archive = tarfile.open(fileobj=temporary) if temporary else zipfile.ZipFile(path) if path.suffix == ".zip" else tarfile.open(path) if path.name.endswith(".tar.xz") else None
    if archive is None:
        return dict(inventory=None, files=None, unpacked_bytes=None, formats={}, license_documents=[])
    with (temporary or nullcontext()), archive, gzip.open(output, "wt", encoding="utf-8") as dest:
        entries = archive.infolist() if isinstance(archive, zipfile.ZipFile) else archive.getmembers()
        total, count = 0, 0
        for entry in entries:
            is_zip = isinstance(archive, zipfile.ZipFile)
            if entry.is_dir() if is_zip else not entry.isfile():
                continue
            name = entry.filename if is_zip else entry.name
            if Path(name).is_absolute() or ".." in Path(name).parts:
                raise ValueError("Unsafe archive member: " + name)
            size = entry.file_size if is_zip else entry.size
            with archive.open(entry) if is_zip else archive.extractfile(entry) as source:
                sha = digest(source)  # ZipFile also checks CRC while reading.
            record = dict(path=name, bytes=size, sha256=sha)
            dest.write(json.dumps(record, ensure_ascii=False) + "\n")
            total += size; count += 1; formats[Path(name).suffix.lower() or "(none)"] += 1
            leaf = Path(name).name.lower()
            if any(x in leaf for x in ("license", "copying", "copyright", "readme", "attribution", "credits")) or Path(name).suffix.lower() in (".md", ".txt"):
                if c['id'] == 'Korean_Gugak':
                    # Preserve restricted publisher manuals in the private
                    # original archive; their public redistribution is not granted.
                    docs.append(dict(external_archive=path.relative_to(data_root).as_posix(), original_member=name, sha256=sha))
                    continue
                relative = Path("docs/soundbanks/licenses") / c["id"] / path.name / name
                target = REPO / relative
                target.parent.mkdir(parents=True, exist_ok=True)
                with archive.open(entry) if is_zip else archive.extractfile(entry) as source:
                    target.write_bytes(source.read())
                docs.append(dict(path=relative.as_posix(), original_member=name, sha256=sha))
        return dict(inventory=output.relative_to(data_root).as_posix(), files=count,
                    unpacked_bytes=total, formats=dict(formats), license_documents=docs)

def gugak_collection(data, private_bundle=None):
    c = dict(id="Korean_Gugak", name="Gugak EXS24 — Korean Instruments", publisher="Seoul Center for Arts & Technology (formerly CATSNU / Seoul National University)", source="https://www.catseoul.com/gugak-library/", original_source="http://en.catsnu.com/Project/GugakVSTi.aspx", bank_type="USER", suggested_directory="instruments/USER/soundfont/Korean_Gugak/", dedicated_collection=True, license="LicenseRef-CATSeoul-Personal-Noncommercial", factory_status="excluded_restricted_personal_use", attribution="Preserve the original Korean acknowledgement in docs/soundbanks/licenses/Korean_Gugak/publisher-terms-20261014.txt on derivative compositions/edits, as required by the publisher.", redistribution_conditions="Personal noncommercial use only. Published notice does not grant library redistribution; never put these archives/samples in Git, public releases or Factory packages.", provenance="Original successor publisher identifies itself as former Seoul National University Center for Arts and Technologies and provides 23 original EXS24 folders.", rank=9, artifacts=[], collection_status="not_acquired")
    c['license_scope_note'] = 'The National Gugak Center digital-sample service separately announces KOGL Type 1. That notice has not been verified as applying to these CATSNU/CATSeoul EXS24 files; retain their own notice and attribution.'
    c['separate_type1_source'] = 'https://www.gugak.go.kr/site/program/board/basicboard/view?boardid=42216&boardtypeid=60&currentpage=22&lang=ko&menuid=001005006002&pagesize=10'
    acquisition = data / "Korean_Gugak/acquisition.json"
    if not acquisition.exists():return c
    for item in json.loads(acquisition.read_text())["artifacts"]:
        a = dict(item)
        a.update(source_version="unversioned publisher Dropbox folder; acquisition snapshot 2026-10-14", variant="exs", original_archive_unchanged=True, container_identity_note="Dropbox-generated ZIP wrapper timestamps can differ on re-download; member SHA-256 inventory is the content identity.")
        if a["status"] == "acquired_archive_verified":
            path = data / a["relative_path"]
            with path.open("rb") as source:
                if digest(source) != a["sha256"]:raise ValueError("Acquired Gugak archive changed: " + str(path))
            a.update(inventory(c, path, data))
        c["artifacts"].append(a)
    c["collection_status"] = "all_23_publisher_EXS24_folders_acquired" if len(c["artifacts"])==23 and all(a["status"]=="acquired_archive_verified" for a in c["artifacts"]) else "acquisition_incomplete"
    if private_bundle:
        base = Path(private_bundle) / 'instruments/USER/soundfont/Korean_Gugak'
        acquired = json.loads((base / 'acquisition-manifest.json').read_text())
        prepared = json.loads((base / 'disk-image-files.json').read_text())
        c['publisher_files'] = [{**x, 'path': x['path'].replace('SoundBanks/', 'instruments/USER/soundfont/', 1).split('instruments/USER/soundfont/', 1)[-1]} for x in acquired['files'] if x['role'] == 'original_publisher_file']
        c['disk_image_preparation'] = dict(method=prepared['note'], file_inventory='Korean_Gugak/disk-image-files.json', files=len(prepared['files']), bytes=sum(x['bytes'] for x in prepared['files']), exs_definitions=sum(x['path'].lower().endswith('.exs') for x in prepared['files']))
        packaging = data / 'results/gugak-packaging.json'
        if packaging.exists():c['packaging_inventory'] = json.loads(packaging.read_text())
    return c

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("data_root", type=Path)
    parser.add_argument("--private-bundle", type=Path, help="Include prepared Gugak original-file inventory from the private sideload bundle")
    args = parser.parse_args(); data = args.data_root.resolve()
    manifest = dict(schema_version=1, acquisition_date=DATE, bank_type="USER",
                    storage_root=str(data), factory_packaging_implemented=False,
                    collections=[], notes=["Storage figures include alternate distributions of the same recordings; they are not unique audio totals.",
                      "All original archives remain unchanged. External inventories contain every archive member's SHA-256.",
                      "Loaded means offline preparation plus a short finite-render smoke test, not full format fidelity."])
    for definition in CANDIDATES:
        c = dict(definition); specs = c.pop("artifacts"); c["artifacts"] = []
        c["bank_type"] = "USER"; c["suggested_directory"] = "instruments/USER/soundfont/" + c["id"]
        c["factory_status"] = "requires_provenance_review" if c["id"] in ("GeneralUser_GS", "Stargate") else "license_candidate_pending_compatibility_review"
        c["redistribution_conditions"] = c["attribution"]
        for filename, version, variant in specs:
            print("Inventory:", filename, flush=True)
            path = data / filename
            a = dict(filename=filename, source_version=version, variant=variant,
                     download_url=url_for(c, filename, version), acquisition_date=DATE)
            try:
                with path.open("rb") as stream: a["sha256"] = digest(stream)
                a["download_bytes"] = path.stat().st_size
                a.update(inventory(c, path, data)); a["status"] = "acquired_archive_verified"
            except (OSError, ValueError, zipfile.BadZipFile, tarfile.TarError) as error:
                a["status"] = "incomplete_or_unavailable"; a["error"] = str(error)
            c["artifacts"].append(a)
        if c["id"] == "Akai_S612_Modular": c["collection_status"] = "repository_acquired_original_collection_incomplete"
        else: c["collection_status"] = "listed_distributions_acquired" if all(a["status"] == "acquired_archive_verified" for a in c["artifacts"]) else "acquisition_incomplete"
        manifest["collections"].append(c)
    manifest["collections"].append(gugak_collection(data, args.private_bundle))
    if args.private_bundle:
        manifest['private_bundle_name'] = args.private_bundle.name
        manifest['private_bundle_note'] = 'Sideloaded includes attributable libraries too large to package with the app; all source licenses and private-data boundaries remain applicable.'
    (REPO / "soundbank_manifest.json").write_text(json.dumps(manifest, indent=2, ensure_ascii=False) + "\n")

if __name__ == "__main__": main()
