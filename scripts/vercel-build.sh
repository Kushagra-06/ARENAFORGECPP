#!/usr/bin/env bash
# OPTIONAL and UNTESTED: build the WebAssembly bundle on Vercel's build machine,
# so you don't need Emscripten locally. To try it, set Vercel's Build Command
# (Project Settings -> Build & Development) to:  bash scripts/vercel-build.sh
# Requires `make` and `python3` on the build image.
set -euo pipefail
cd "$(dirname "$0")/.."

if ! command -v em++ >/dev/null 2>&1; then
  [ -d .emsdk ] || git clone --depth 1 https://github.com/emscripten-core/emsdk.git .emsdk
  ./.emsdk/emsdk install latest
  ./.emsdk/emsdk activate latest
  # shellcheck disable=SC1091
  source ./.emsdk/emsdk_env.sh
fi

make site
