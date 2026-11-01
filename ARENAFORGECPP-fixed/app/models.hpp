#pragma once
#include <string>
#include <map>
#include <vector>

namespace arena {
    struct Team {
        int id;
        std::string name;
        double rating;
    };

    enum class Format { Knockout = 0, RoundRobin = 1, Swiss = 2 };
    enum class TStatus { Registration = 0, InProgress = 1, Completed = 2 };
    enum class RegResult { Registered, Waitlisted };

    struct Seed { int teamId; double rating; };
    struct ByeRecord { int round; int teamId; };
    
    struct Standing {
        int teamId = 0;
        int played = 0, wins = 0, losses = 0, diff = 0;
        double key = 0;   /* value handed to std::sort */
    };
    
    using TeamBook = std::map<int, Team>;
}
