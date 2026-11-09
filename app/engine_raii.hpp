#pragma once
#include "arena_engine.hpp"
#include <algorithm>

namespace arena {
    // Wrapper for sorting that delegates to std::sort with our custom comparator
    inline void sortRankEntries(std::vector<RankEntry>& entries) {
        std::sort(entries.begin(), entries.end(), rankEntryComparator);
    }

    // Modern wrappers delegating to the new C++ engine classes
    using DirectoryWrapper = TeamDirectory;
    using WaitlistWrapper = Waitlist;
    using HistoryWrapper = MatchHistory;
    using BracketWrapper = BracketTree;
}
