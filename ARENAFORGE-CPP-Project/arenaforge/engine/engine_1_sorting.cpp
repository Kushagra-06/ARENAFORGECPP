#include "arena_engine.hpp"
#include <algorithm>

namespace arena {
    bool rankEntryComparator(const RankEntry& a, const RankEntry& b) {
        if (a.score != b.score) return a.score > b.score;
        return a.id < b.id; // Breaks ties using the lower ID
    }
}
