# ArenaForge

A smart MMR-based esports tournament manager. The engine (sorting, heap matchmaking,
bracket tree, hash table, queue, linked list, graph colouring) is C++17 and runs in the
browser as WebAssembly. The page in `web/index.html` seeds teams by rating and runs a
knockout bracket on top of it.

Only the **Knockout** format is implemented in the engine so far; Round Robin and Swiss
are not, and the page shows them as unavailable.

## Layout

| Path | What it is |
|------|------------|
| `engine/` | Data-structure engine (`arena_engine.hpp` + one `.cpp` per structure) |
| `app/` | Tournament layer (`ArenaSystem`) and the Emscripten bindings (`web_api.cpp`) |
| `tests/` | Native test suite |
| `web/` | The static site Vercel serves: `index.html`, plus `arena.js` once built |

## Test natively

```bash
make test
```

## Build the web app

Needs [Emscripten](https://emscripten.org/docs/getting_started/downloads.html) (`em++`),
`make` and `python3`. On Windows, use WSL.

```bash
make site          # builds build/arena.js and copies it to web/arena.js
git add web/arena.js
git commit -m "Build WebAssembly bundle" && git push
```

`web/arena.js` is a single file with the WebAssembly embedded, so there is no separate
`.wasm` to deploy. Vercel serves `web/` as static files (see `vercel.json`) and does not
compile any C++.

Alternatively, `scripts/vercel-build.sh` can build the bundle on Vercel itself (untested;
see the comments in that file).
