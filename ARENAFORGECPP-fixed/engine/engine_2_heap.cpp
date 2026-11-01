#include "arena_engine.hpp"
#include <cmath>
#include <unordered_set>

namespace arena {
    double calculateFairScore(double rating_a, double rating_b, int prev_meetings) {
        return std::abs(rating_a - rating_b) + (AE_REPEAT_PENALTY * prev_meetings);
    }

    int matchmake(const std::vector<RankEntry>& teams, const MatchHistory& hist, 
                  std::vector<MatchPair>& out, int max_out) {
        std::priority_queue<MatchPair, std::vector<MatchPair>, std::greater<MatchPair>> pq;
        int n = teams.size();
        
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                double cost = calculateFairScore(teams[i].score, teams[j].score, 
                                                 hist.meetings(teams[i].id, teams[j].id));
                pq.push({teams[i].id, teams[j].id, cost});
            }
        }

        std::unordered_set<int> used;
        int count = 0;
        
        while (!pq.empty() && count < max_out) {
            MatchPair p = pq.top();
            pq.pop();
            
            if (used.count(p.team_a) || used.count(p.team_b)) continue;
            
            used.insert(p.team_a);
            used.insert(p.team_b);
            out.push_back(p);
            count++;
        }
        return count;
    }
}
