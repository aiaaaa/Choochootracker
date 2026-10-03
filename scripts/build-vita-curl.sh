#!/bin/bash
# Rebuild curl against the image's actual OpenSSL (the prebuilt curl has an ABI mismatch).
set -euo pipefail
work=${VITA_CURL_WORK:-/deps/curl}
mkdir -p "$work"
archive="$work/curl-8.17.0.tar.gz"
if [[ ! -f "$archive" ]]; then
  curl --fail --location --proto '=https' --proto-redir '=https' --max-redirs 3 \
    --connect-timeout 10 --max-time 120 --max-filesize 16777216 \
    https://github.com/curl/curl/releases/download/curl-8_17_0/curl-8.17.0.tar.gz -o "$archive.part"
  mv "$archive.part" "$archive"
fi
echo "e8e74cdeefe5fb78b3ae6e90cd542babf788fa9480029cfcee6fd9ced42b7910  $archive" | sha256sum -c -
if [[ ! -d "$work/source" ]]; then
  mkdir "$work/source"
  tar xf "$archive" --strip-components=1 -C "$work/source"
fi
cmake -S "$work/source" -B "$work/build" \
  -DCMAKE_TOOLCHAIN_FILE="$VITASDK/share/vita.toolchain.cmake" \
  -DCMAKE_INSTALL_PREFIX="$work/prefix" -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_CURL_EXE=OFF -DBUILD_SHARED_LIBS=OFF -DBUILD_TESTING=OFF \
  -DENABLE_IPV6=OFF -DCURL_DISABLE_SOCKETPAIR=ON -DHAVE_FCNTL_O_NONBLOCK=OFF \
  -DENABLE_THREADED_RESOLVER=ON -DBUILD_LIBCURL_DOCS=OFF -DBUILD_MISC_DOCS=OFF \
  -DENABLE_CURL_MANUAL=OFF -DCURL_USE_LIBPSL=OFF -DCURL_ZSTD=OFF -DCURL_BROTLI=OFF \
  -DHTTP_ONLY=ON -DCURL_USE_OPENSSL=ON -DCMAKE_C_FLAGS=-DOPENSSL_NO_UI_CONSOLE \
  -DOPENSSL_ROOT_DIR="$VITASDK/arm-vita-eabi" -DCURL_CA_BUNDLE=app0:/certs/ca-certificates.crt
# SDK OpenSSL omits its console password UI; this is unrelated to TLS verification.
# Vita newlib declares pipe2 but does not implement it; official SDK recipe does this too.
sed -i '/define HAVE_PIPE2 /d' "$work/build/lib/curl_config.h"
cmake --build "$work/build" -j"${VITA_JOBS:-4}"
cmake --install "$work/build"
