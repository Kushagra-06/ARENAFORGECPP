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
        // State is returned as a JSON string, so no register_vector is needed.
        .function("getStateJson", &arena::ArenaSystem::getStateJson)
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
