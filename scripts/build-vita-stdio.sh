#!/bin/bash
# Build one existing newlib object; never replace or modify the installed SDK.
set -euo pipefail
work=${VITA_STDIO_WORK:-/deps/newlib}
recipe_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
read -r revision checksum < <(python3 - "$recipe_dir/vita-sdk.json" <<'PY'
import json,sys
p=json.load(open(sys.argv[1]))['newlib_c99_scanner']
print(p['revision'],p['archive_sha256'])
PY
)
mkdir -p "$work"
archive="$work/newlib-$revision.tar.gz"
if [[ ! -f "$archive" ]]; then
  curl --fail --location --proto '=https' --proto-redir '=https' --max-redirs 3 \
    --connect-timeout 10 --max-time 120 --max-filesize 67108864 \
    "https://codeload.github.com/vitasdk/newlib/tar.gz/$revision" -o "$archive.part"
  mv "$archive.part" "$archive"
fi
echo "$checksum  $archive" | sha256sum -c -
if [[ ! -d "$work/source" ]]; then
  mkdir "$work/source"
  tar xf "$archive" --strip-components=1 -C "$work/source"
fi
# Internal stdio objects are ABI-sensitive. An SDK upgrade must deliberately
# select matching source and revalidate this small override, not silently reuse it.
printf '#include <_newlib_version.h>\n#if __NEWLIB__ != 4 || __NEWLIB_MINOR__ != 1 || __NEWLIB_PATCHLEVEL__ != 0\n#error Review C99 scanner for this newlib ABI\n#endif\n' \
  | arm-vita-eabi-gcc -x c -fsyntax-only -
arm-vita-eabi-gcc -Os -D_WANT_IO_C99_FORMATS=1 -DSTRING_ONLY \
  -ffunction-sections -fdata-sections \
  -c "$work/source/newlib/libc/stdio/vfscanf.c" -o "$work/c99-scanf.o"
cp "$work/source/COPYING.NEWLIB" "$work/LICENSE.newlib.txt"
# Include the scanner's own Berkeley notice alongside the full newlib notices.
sed -n '1,/^ \*\//p' "$work/source/newlib/libc/stdio/vfscanf.c" > "$work/LICENSE.scanner.txt"
