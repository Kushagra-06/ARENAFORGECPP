#include "arena_system.hpp"
#include <algorithm>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace arena {
    
    void ArenaSystem::registerTeam(const std::string& name, double rating) {
        int id = teams.size() + 1;
        teams[id] = {id, name, rating};
        dir.put(name, id);
        waitlist.enqueue(id);
    }
    
    void ArenaSystem::startTournament(Format f) {
        // Only Knockout is implemented, and it needs at least two teams.
        if (status != TStatus::Registration) return;
        if (f != Format::Knockout || teams.size() < 2) return;
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
        if (status != TStatus::InProgress || format != Format::Knockout) return;
        if (bracket.reportWinner(matchId, winnerId) && bracket.getChampion() >= 0) {
            status = TStatus::Completed;
        }
    }
    
    std::vector<Standing> ArenaSystem::getStandings() const {
        std::vector<Standing> stds;
        for (const auto& [id, team] : teams) {
            stds.push_back({id, 0, 0, 0, 0, team.rating});
        }
        return stds;
    }

    namespace {
        void appendJsonString(std::ostringstream& os, const std::string& in) {
            os << '"';
            for (unsigned char c : in) {
                switch (c) {
                    case '"':  os << "\\\""; break;
                    case '\\': os << "\\\\"; break;
                    default:
                        if (c < 0x20) {
                            char buf[8];
                            std::snprintf(buf, sizeof buf, "\\u%04x", c);
                            os << buf;
                        } else {
                            os << static_cast<char>(c);
                        }
                }
            }
            os << '"';
        }

        void collectMatches(const BracketNode* n, std::vector<const BracketNode*>& out) {
            if (!n || n->node_id <= 0) return;
            collectMatches(n->left.get(), out);
            collectMatches(n->right.get(), out);
            out.push_back(n);
        }
    }

    std::string ArenaSystem::getStateJson() const {
        std::ostringstream os;
        os << std::setprecision(15);

        os << "{\"status\":" << static_cast<int>(status)
           << ",\"format\":" << static_cast<int>(format)
           << ",\"champion\":" << bracket.getChampion();

        os << ",\"teams\":[";
        bool first = true;
        for (const auto& [id, team] : teams) {
            if (!first) os << ',';
            first = false;
            double r = std::isfinite(team.rating) ? team.rating : 0.0;
            os << "{\"id\":" << id << ",\"name\":";
            appendJsonString(os, team.name);
            os << ",\"rating\":" << r << '}';
        }
        os << ']';

        std::vector<RankEntry> entries;
        for (const auto& [id, team] : teams) entries.push_back({id, team.rating});
        sortRankEntries(entries);
        os << ",\"seeds\":[";
        for (size_t i = 0; i < entries.size(); ++i) {
            if (i) os << ',';
            os << entries[i].id;
        }
        os << ']';

        std::vector<const BracketNode*> nodes;
        collectMatches(bracket.getRoot(), nodes);
        std::sort(nodes.begin(), nodes.end(),
                  [](const BracketNode* a, const BracketNode* b) { return a->node_id < b->node_id; });
        os << ",\"matches\":[";
        for (size_t i = 0; i < nodes.size(); ++i) {
            const BracketNode* n = nodes[i];
            bool ready = n->team_id == AE_TBD && n->left->team_id >= 0 && n->right->team_id >= 0;
            if (i) os << ',';
            os << "{\"id\":" << n->node_id
               << ",\"round\":" << n->round
               << ",\"a\":" << n->left->team_id
               << ",\"b\":" << n->right->team_id
               << ",\"fromA\":" << n->left->node_id
               << ",\"fromB\":" << n->right->node_id
               << ",\"w\":" << n->team_id
               << ",\"ready\":" << (ready ? "true" : "false") << '}';
        }
        os << "]}";
        return os.str();
    }
}
