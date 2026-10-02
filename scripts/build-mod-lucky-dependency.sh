#!/bin/sh
# Build only the pinned native module backend, using the caller's existing C toolchain.
set -eu
lucky_repo=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
lucky_work=${MOD_LUCKY_DEPS_DIR:-"$lucky_repo/.tmp/mod-lucky"}
lucky_prefix=${MOD_LUCKY_PREFIX:-"$lucky_work/prefix"}
lucky_version=4.7.3
lucky_hash=b6a98797e4fb9c9a705f5d53112aa5214561857e929a644928b9e658930d9440
mkdir -p "$lucky_work"
lucky_work=$(CDPATH= cd -- "$lucky_work" && pwd)
mkdir -p "$lucky_prefix"
lucky_prefix=$(CDPATH= cd -- "$lucky_prefix" && pwd)
lucky_archive=${MOD_LUCKY_ARCHIVE:-"$lucky_work/libxmp-$lucky_version.tar.gz"}
if [ ! -f "$lucky_archive" ]; then
  curl --fail --location --proto '=https' --proto-redir '=https' --max-redirs 3 \
    --connect-timeout 10 --max-time 120 --max-filesize 5242880 \
    --user-agent 'ChooChooTracker-ModLucky-build/0.1' \
    "https://codeload.github.com/libxmp/libxmp/tar.gz/refs/tags/libxmp-$lucky_version" \
    --output "$lucky_archive.part"
  mv "$lucky_archive.part" "$lucky_archive"
fi
if command -v sha256sum >/dev/null 2>&1; then
  lucky_actual=$(sha256sum "$lucky_archive" | cut -d ' ' -f 1)
else
  lucky_actual=$(shasum -a 256 "$lucky_archive" | cut -d ' ' -f 1)
fi
[ "$lucky_actual" = "$lucky_hash" ] || { echo 'libxmp archive checksum mismatch' >&2; exit 1; }
lucky_source="$lucky_work/libxmp-$lucky_version"
if [ ! -d "$lucky_source" ]; then
  mkdir "$lucky_source"
  tar -xzf "$lucky_archive" --strip-components=1 -C "$lucky_source"
fi
cd "$lucky_source"
# Pass e.g. --host=aarch64-linux-gnu only with an already configured cross toolchain.
./configure --enable-lite --enable-static --disable-shared --disable-depackers \
  --disable-prowizard --prefix="$lucky_prefix" "$@"
make -j"${MOD_LUCKY_JOBS:-4}" static-lite
mkdir -p "$lucky_prefix/include" "$lucky_prefix/lib" "$lucky_prefix/share/licenses/libxmp"
cp include/xmp.h "$lucky_prefix/include/"
cp lib/libxmp-lite.a "$lucky_prefix/lib/"
cp "$lucky_repo/tracker/src/experimental/mod_lucky/notices/LICENSE.libxmp.txt" "$lucky_prefix/share/licenses/libxmp/"
printf 'libxmp-lite %s ready in %s\n' "$lucky_version" "$lucky_prefix"
