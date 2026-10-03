#!/bin/bash
# Internal entry point, called only by vita.py in the pinned SDK.
set -euo pipefail
profile=$1
export PKG_CONFIG_LIBDIR="$VITASDK/arm-vita-eabi/lib/pkgconfig"
flag=0
if [[ "$profile" == personal ]]; then
  flag=1
  if [[ ! -f /deps/curl/prefix/lib/libcurl.a ]]; then /src/scripts/build-vita-curl.sh; fi
  if [[ ! -f /deps/target/prefix/lib/libxmp-lite.a ]]; then
    CC=arm-vita-eabi-gcc AR=arm-vita-eabi-ar RANLIB=arm-vita-eabi-ranlib \
      MOD_LUCKY_DEPS_DIR=/deps/target MOD_LUCKY_PREFIX=/deps/target/prefix \
      /src/scripts/build-mod-lucky-dependency.sh --host=arm-vita-eabi
  fi
fi
make -C /src/tracker -f Makefile.vita -j"${VITA_JOBS:-4}" BUILD=/out/native \
  CHOOCHOO_EXPERIMENTAL_MOD_LUCKY="$flag" MOD_LUCKY_PREFIX=/deps/target/prefix vita
# Ordinary host regression tests run in this same immutable environment. The
# Lucky-enabled host suite is a separate host-toolchain command (see vita.md).
make -C /src/tracker -f Makefile.test -j"${VITA_JOBS:-4}" BUILD_DIR=/out/tests \
  CHOOCHOO_EXPERIMENTAL_MOD_LUCKY=0 /out/tests/run_tests
mkdir -p /out/build/tests
ln -sfn /src/tracker/packaging /out/packaging
(cd /out && ./tests/run_tests)
python3 /src/scripts/vita-package.py "$profile"
