#include "arena_system.hpp"
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(arenaforge_module) {
    class_<arena::ArenaSystem>("ArenaSystem")
        .constructor<>()
        .function("registerTeam", &arena::ArenaSystem::registerTeam)
        .function("startTournament", &arena::ArenaSystem::startTournament)
        .function("reportResult", &arena::ArenaSystem::reportResult)
        // Note: For real WASM binding, std::vector return types require register_vector
        ;
        
    enum_<arena::Format>("Format")
        .value("Knockout", arena::Format::Knockout)
        .value("RoundRobin", arena::Format::RoundRobin)
        .value("Swiss", arena::Format::Swiss);
}
#else

int main() {
    std::cout << "ArenaForge Core Web API initialized." << std::endl;
    return 0;
}

#endif
