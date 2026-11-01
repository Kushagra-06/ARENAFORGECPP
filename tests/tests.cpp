#include "../app/arena_system.hpp"
#include <iostream>

int main() {
    std::cout << "Running ArenaForge Tests..." << std::endl;
    arena::ArenaSystem sys;
    sys.registerTeam("Team A", 1500.0);
    sys.registerTeam("Team B", 1600.0);
    sys.startTournament(arena::Format::Knockout);
    std::cout << "Basic registration and tournament start OK." << std::endl;
    return 0;
}
