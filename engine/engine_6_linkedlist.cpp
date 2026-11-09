#include "arena_engine.hpp"

namespace arena {
    void MatchHistory::append(const HistNode& rec) {
        history.push_back(rec);
    }

    int MatchHistory::meetings(int a, int b) const {
        int c = 0;
        for (const auto& n : history) {
            if ((n.team_a == a && n.team_b == b) || (n.team_a == b && n.team_b == a)) {
                c++;
            }
        }
        return c;
    }

    const std::vector<HistNode>& MatchHistory::getHistory() const {
        return history;
    }

    size_t MatchHistory::count() const {
        return history.size();
    }
}
