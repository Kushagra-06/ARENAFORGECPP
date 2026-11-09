#pragma once
#include "engine_raii.hpp"
#include "models.hpp"
#include <cmath>
#include <string>
#include <utility>
#include <vector>
#include <map>

namespace arena {

    class ArenaSystem {
    private:
        TeamBook teams;
        DirectoryWrapper dir;
        WaitlistWrapper waitlist;
        HistoryWrapper history;
        BracketWrapper bracket;
        
        Format format;
        TStatus status;
        
    public:
        ArenaSystem() : format(Format::Knockout), status(TStatus::Registration) {}
        
        void registerTeam(const std::string& name, double rating);
        void startTournament(Format f);
        void reportResult(int matchId, int winnerId);
        std::vector<Standing> getStandings() const;

        // Full tournament state as a JSON string (used by the web front-end):
        // {status, format, champion, teams[], seeds[], matches[]}
        std::string getStateJson() const;
    };
}
