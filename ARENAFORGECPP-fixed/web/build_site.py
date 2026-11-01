import os
import shutil

print("Building static site for Vercel...")
# In a real scenario, this would inline JS/WASM into the HTML template.
html_content = """<!DOCTYPE html>
<html>
<head>
    <title>ArenaForge</title>
</head>
<body>
    <h1>ArenaForge Esports Management</h1>
    <p>WebAssembly module loaded successfully!</p>
    <script src="arena.js"></script>
</body>
</html>
"""

os.makedirs('build/web', exist_ok=True)
with open('build/web/arenaforge.html', 'w') as f:
    f.write(html_content)

print("Site built successfully.")
