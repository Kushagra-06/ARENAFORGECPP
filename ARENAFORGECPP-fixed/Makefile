# ArenaForge build  
# Native build of the C++ engine + C++ application layer, runs the test suite
# make test    - native build & run tests
# make site    - wasm + inline everything into one web/arenaforge.html

CXX := g++  
CXXFLAGS := -std=c++17 -O1 -g -Wall -Wextra -Iengine -Iapp  
SAN := -fsanitize=address,undefined -fno-omit-frame-pointer  

ENGINE_SRC := $(wildcard engine/*.cpp)  
APP_SRC := app/tournament.cpp app/arena_system.cpp app/snapshot.cpp  

build/%.o: engine/%.cpp | build  
	$(CXX) $(CXXFLAGS) $(SAN) -c $< -o $@  

build:  
	mkdir -p build  

ENGINE_OBJ := $(patsubst engine/%.cpp,build/%.o,$(ENGINE_SRC))  

build/arenaforge_tests: $(ENGINE_OBJ) $(APP_SRC) tests/tests.cpp app/*.hpp engine/arena_engine.hpp  
	$(CXX) $(CXXFLAGS) $(SAN) $(ENGINE_OBJ) $(APP_SRC) tests/tests.cpp -o $@  

test: build/arenaforge_tests  
	./build/arenaforge_tests  

# ---- WebAssembly: All C++ sources ----  
WASM_FLAGS := -O2 -fexceptions -sMODULARIZE=1 -sEXPORT_NAME=ArenaModule -sSINGLE_FILE=1 \  
 -sFORCE_FILESYSTEM=1 -sALLOW_MEMORY_GROWTH=1 -sENVIRONMENT=web,node \  
 -sEXPORTED_RUNTIME_METHODS=ccall,FS  

wasm: build/arena.js build/arena_js.js  

build/arena.js: $(ENGINE_SRC) $(APP_SRC) app/web_api.cpp app/*.hpp engine/arena_engine.hpp  
	mkdir -p build/wasm  
	for f in $(ENGINE_SRC) $(APP_SRC) app/web_api.cpp; do em++ -std=c++17 -O2 -fexceptions -Iengine -Iapp -c $$f -o build/wasm/$$(basename $$f .cpp).o || exit 1; done  
	em++ $(WASM_FLAGS) build/wasm/*.o -o $@  

site: build/arena.js build/arena_js.js  
	python3 web/build_site.py  

JS_FLAGS := -O2 -fexceptions -sWASM=0 -sMODULARIZE=1 -sEXPORT_NAME=ArenaModuleJS -sSINGLE_FILE=1 \  
 -sFORCE_FILESYSTEM=1 -sALLOW_MEMORY_GROWTH=1 -sENVIRONMENT=web,node \  
 -sEXPORTED_RUNTIME_METHODS=ccall,FS  

build/arena_js.js: build/arena.js  
	em++ $(JS_FLAGS) build/wasm/*.o -o $@  

clean:  
	rm -rf build  

.PHONY: test wasm site clean  
