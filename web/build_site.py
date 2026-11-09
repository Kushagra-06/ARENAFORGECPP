"""Copy the Emscripten build output into web/ so Vercel can serve it.

Run through `make site`, which builds build/arena.js first. web/index.html is
hand-written and loads arena.js (and arena_js.js, if present, as a fallback).
"""
import shutil
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BUILD = ROOT / "build"
WEB = ROOT / "web"


def main() -> int:
    if not (WEB / "index.html").is_file():
        print("error: web/index.html is missing", file=sys.stderr)
        return 1
    if not (BUILD / "arena.js").is_file():
        print("error: build/arena.js not found. Run `make site` (needs Emscripten's em++).", file=sys.stderr)
        return 1

    for name in ("arena.js", "arena_js.js"):          # arena_js.js only exists after `make wasm`
        src = BUILD / name
        if src.is_file():
            shutil.copy2(src, WEB / name)
            print(f"copied {name} -> web/{name} ({src.stat().st_size / 1024:.0f} KB)")

    print("Site ready in web/. Commit web/arena.js and push to deploy.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
