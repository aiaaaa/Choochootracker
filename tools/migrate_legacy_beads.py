#!/usr/bin/env python3
"""Convert explicitly identified pre-Oct-14 personal Beads songs, without overwriting.

Stock Trance Gate also uses 14: provenance MUST be supplied by the caller.
Only the module field changes (14 -> 15); audio and all other song data stay intact.
"""
import argparse
import copy
import hashlib
import io
import json
from pathlib import Path
import re
import zipfile


def migrate_text(data):
    if not data.startswith((b"# ChooChooTracker Module", b"# ChipNomad Tracker Module")):
        raise ValueError("Not a tracker project")
    changes = []
    output = []
    for line_number, line in enumerate(data.splitlines(keepends=True), 1):
        prefix = next((p for p in (b"- Insert: ", b"- Master insert: ")
                       if line.startswith(p)), None)
        if prefix:
            body = line[len(prefix):].rstrip(b"\r\n")
            ending = line[len(prefix) + len(body):]
            fields = body.split(b",")
            master = prefix == b"- Master insert: "
            module_index = 1 if master else 2
            if len(fields) != (11 if master else 12) or any(
                    not re.fullmatch(rb"[0-9]+", f) for f in fields):
                raise ValueError(f"Malformed insert on line {line_number}")
            values = list(map(int, fields))
            if (values[0] > (1 if master else 7)
                    or (not master and values[1] > 1)
                    or values[module_index + 1] > 1
                    or any(v > 255 for v in values[module_index + 2:])):
                raise ValueError(f"Invalid insert fields on line {line_number}")
            if values[module_index] > 14:
                raise ValueError("Newer/converted insert catalogue; refusing legacy conversion")
            if values[module_index] == 14:
                fields[module_index] = b"15"
                line = prefix + b",".join(fields) + ending
                changes.append({"line": line_number, "scope": "master" if master else "track",
                                "track": None if master else values[0],
                                "slot": values[0] if master else values[1]})
        output.append(line)
    return b"".join(output), changes


def migrate_bytes(data, *, legacy_personal=False):
    if not legacy_personal:
        raise ValueError("Identify source as legacy personal; stock Trance Gate must not be converted")
    if not data.startswith(b"PK"):
        return migrate_text(data)
    with zipfile.ZipFile(io.BytesIO(data)) as archive:
        entries = archive.infolist()
        names = [entry.filename for entry in entries]
        if len(names) != len(set(names)) or names.count("project.cct") != 1:
            raise ValueError("Ambiguous archive entries or missing project.cct")
        if any(e.compress_type != zipfile.ZIP_STORED or e.flag_bits & 1 for e in entries):
            raise ValueError("Expected unencrypted, stored CCT archive")
        project, changes = migrate_text(archive.read("project.cct"))
        if not changes:
            return data, changes
        buffer = io.BytesIO()
        with zipfile.ZipFile(buffer, "w") as result:
            result.comment = archive.comment
            for entry in entries:
                payload = project if entry.filename == "project.cct" else archive.read(entry)
                result.writestr(copy.copy(entry), payload)
        converted = buffer.getvalue()
        with zipfile.ZipFile(io.BytesIO(converted)) as check:
            if check.testzip() is not None:
                raise ValueError("Converted archive failed CRC verification")
            for entry in entries:
                expected = project if entry.filename == "project.cct" else archive.read(entry)
                if check.read(entry.filename) != expected:
                    raise ValueError("Converted archive changed an unrelated payload")
        return converted, changes


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path)
    parser.add_argument("--legacy-personal", action="store_true", required=True,
                        help="Source came from the old personal build with Beads=14")
    parser.add_argument("--output", type=Path,
                        help="Write a new converted copy; omit to inspect only")
    args = parser.parse_args()
    source = args.source.read_bytes()
    converted, changes = migrate_bytes(source, legacy_personal=args.legacy_personal)
    if args.output and changes:
        # Exclusive creation protects originals and previous migration outputs.
        with args.output.open("xb") as output:
            output.write(converted)
    print(json.dumps({"source": str(args.source), "output": str(args.output) if changes and args.output else None,
                      "source_sha256": hashlib.sha256(source).hexdigest(),
                      "converted_sha256": hashlib.sha256(converted).hexdigest(),
                      "changed_slots": changes, "original_preserved": True}, indent=2))


if __name__ == "__main__":
    main()
