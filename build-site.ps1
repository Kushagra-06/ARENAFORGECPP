# Windows (PowerShell) equivalent of `make site`. The commands mirror the Makefile.
# Run from the repo root, in a terminal where emsdk_env.ps1 has been sourced
# (so `em++` works):   .\build-site.ps1
Set-Location $PSScriptRoot

if (-not (Get-Command em++ -ErrorAction SilentlyContinue)) {
    throw "em++ not found. Run:  . `$HOME\emsdk\emsdk_env.ps1   (then re-run this script)"
}
if (-not (Test-Path "Makefile") -or -not (Test-Path "engine") -or -not (Test-Path "web\index.html")) {
    throw "Run this from the repo root (the folder containing Makefile, engine\, app\, web\)."
}

$sources = @(
    "engine/engine_1_sorting.cpp", "engine/engine_2_heap.cpp", "engine/engine_3_tree.cpp",
    "engine/engine_4_hashtable.cpp", "engine/engine_5_queue.cpp", "engine/engine_6_linkedlist.cpp",
    "engine/engine_7_graph.cpp",
    "app/tournament.cpp", "app/arena_system.cpp", "app/snapshot.cpp", "app/web_api.cpp"
)

New-Item -ItemType Directory -Force -Path "build/wasm" | Out-Null
$objects = @()
foreach ($src in $sources) {
    $obj = "build/wasm/" + [IO.Path]::GetFileNameWithoutExtension($src) + ".o"
    Write-Host "compiling $src"
    em++ -std=c++17 -O2 -fexceptions -Iengine -Iapp -c $src -o $obj
    if ($LASTEXITCODE -ne 0) { throw "compile failed: $src" }
    $objects += $obj
}

# Every flag is a quoted string: PowerShell would otherwise split "-sX=a,b" at the comma.
$linkArgs = @(
    "-O2", "-fexceptions", "-lembind",
    "-sMODULARIZE=1", "-sEXPORT_NAME=ArenaModule", "-sSINGLE_FILE=1",
    "-sFORCE_FILESYSTEM=1", "-sALLOW_MEMORY_GROWTH=1",
    "-sENVIRONMENT=web,node", "-sEXPORTED_RUNTIME_METHODS=ccall,FS"
)

Write-Host "linking build/arena.js"
em++ @linkArgs @objects -o build/arena.js
if ($LASTEXITCODE -ne 0) { throw "link failed" }

Copy-Item build/arena.js web/arena.js -Force
Write-Host "Done: web/arena.js created. Now commit it and push to main."
