# ARENAFORGE

A Smart MMR-Based Esports Management System.
The core engine has been completely rewritten in modern C++ and configured for WebAssembly deployment on Vercel.

## Building Locally
To test natively:
```bash
make test
```

## Deploying to Vercel
1. Run `make site` locally to generate the WebAssembly module (`arena.js`, `arena.wasm`) and the static site in the `web/` folder.
2. Push your code to GitHub.
3. Vercel will automatically read `vercel.json` and serve your pre-built static files without attempting to run the C++ compiler.
