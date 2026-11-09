#pragma once
#include "arena_system.hpp"

namespace arena {
    class TournamentManager {
    private:
        ArenaSystem system;
    public:
        TournamentManager() {}
        void process();
    };
}
