#include "arena_system.hpp"
#include <iostream>

namespace arena {
    
    void ArenaSystem::registerTeam(const std::string& name, double rating) {
        int id = teams.size() + 1;
        teams[id] = {id, name, rating};
        dir.put(name, id);
        waitlist.enqueue(id);
    }
    
    void ArenaSystem::startTournament(Format f) {
        format = f;
        status = TStatus::InProgress;
        
        std::vector<RankEntry> entries;
        for (const auto& [id, team] : teams) {
            entries.push_back({id, team.rating});
        }
        
        sortRankEntries(entries);
        
        std::vector<int> seeded;
        for (const auto& entry : entries) {
            seeded.push_back(entry.id);
        }
        
        if (format == Format::Knockout) {
            bracket.build(seeded);
        }
    }
    
    void ArenaSystem::reportResult(int matchId, int winnerId) {
        if (format == Format::Knockout) {
            bracket.reportWinner(matchId, winnerId);
        }
        // Simplified reporting for demonstration
    }
    
    std::vector<Standing> ArenaSystem::getStandings() const {
        std::vector<Standing> stds;
        for (const auto& [id, team] : teams) {
            stds.push_back({id, 0, 0, 0, 0, team.rating});
        }
        return stds;
    }
}
