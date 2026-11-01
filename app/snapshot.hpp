#pragma once
#include "arena_system.hpp"

namespace arena {
    void saveSnapshot(const ArenaSystem& sys, const std::string& filename);
    bool loadSnapshot(ArenaSystem& sys, const std::string& filename);
}
