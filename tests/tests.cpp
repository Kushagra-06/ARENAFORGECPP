#include "../app/arena_system.hpp"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

static int failures = 0;
#define CHECK(cond)                                                              \
    do {                                                                         \
        if (!(cond)) {                                                           \
            ++failures;                                                          \
            std::cerr << "FAIL line " << __LINE__ << ": " #cond << std::endl;    \
        }                                                                        \
    } while (0)

static bool contains(const std::string& hay, const std::string& needle) {
    return hay.find(needle) != std::string::npos;
}

// Plays a complete knockout where the better seed (lower id) always wins.
// Invariants: every size finishes, exactly n-1 real matches are played,
// and the top seed is champion.
static void testBracketPlaythrough() {
    for (int n = 2; n <= 40; ++n) {
        std::vector<int> seeds;
        for (int i = 1; i <= n; ++i) seeds.push_back(i);
        arena::BracketTree tree;
        tree.build(seeds);
        CHECK(tree.getChampion() == arena::AE_TBD);

        int played = 0;
        for (int guard = 0; guard < 100; ++guard) {
            std::vector<arena::BracketNode*> ready;
            tree.collectReady(ready, 1000);
            if (ready.empty()) break;
            for (auto* m : ready) {
                int a = m->left->team_id, b = m->right->team_id;
                CHECK(a >= 0 && b >= 0);
                CHECK(!tree.reportWinner(m->node_id, 9999));          // non-participant rejected
                CHECK(tree.reportWinner(m->node_id, std::min(a, b)));
                CHECK(!tree.reportWinner(m->node_id, std::min(a, b))); // can't report twice
                ++played;
            }
        }
        CHECK(played == n - 1);
        CHECK(tree.getChampion() == 1);
    }
}

static void testReportRejections() {
    arena::BracketTree tree;
    tree.build({1, 2, 3, 4});
    CHECK(!tree.reportWinner(-1, 1));   // leaf id
    CHECK(!tree.reportWinner(0, 1));
    CHECK(!tree.reportWinner(99, 1));   // no such match
    CHECK(!tree.reportWinner(3, 1));    // final isn't playable yet
    std::vector<arena::BracketNode*> ready;
    tree.collectReady(ready, 1);
    CHECK(ready.size() == 1);           // max is respected
}

static void testSystemFlow() {
    arena::ArenaSystem sys;
    CHECK(contains(sys.getStateJson(), "\"status\":0"));

    sys.startTournament(arena::Format::Knockout);        // < 2 teams: ignored
    CHECK(contains(sys.getStateJson(), "\"status\":0"));

    sys.registerTeam("Team \"A\" \\ one", 1500.0);       // exercises JSON escaping
    sys.registerTeam("Team B", 1600.0);
    sys.startTournament(arena::Format::RoundRobin);      // not implemented: ignored
    CHECK(contains(sys.getStateJson(), "\"status\":0"));

    sys.startTournament(arena::Format::Knockout);
    std::string s = sys.getStateJson();
    CHECK(contains(s, "\"status\":1"));
    CHECK(contains(s, "\"name\":\"Team \\\"A\\\" \\\\ one\""));
    CHECK(contains(s, "\"seeds\":[2,1]"));               // higher rating seeds first
    CHECK(contains(s, "\"ready\":true"));

    sys.reportResult(1, 99);                              // invalid winner: ignored
    CHECK(contains(sys.getStateJson(), "\"status\":1"));

    sys.reportResult(1, 1);
    s = sys.getStateJson();
    CHECK(contains(s, "\"status\":2"));
    CHECK(contains(s, "\"champion\":1"));

    sys.registerTeam("Late", 1000.0);                     // state stays consistent afterwards
    sys.startTournament(arena::Format::Knockout);
    CHECK(contains(sys.getStateJson(), "\"status\":2"));
}

int main() {
    std::cout << "Running ArenaForge Tests..." << std::endl;
    testBracketPlaythrough();
    testReportRejections();
    testSystemFlow();
    if (failures) {
        std::cerr << failures << " check(s) failed." << std::endl;
        return 1;
    }
    std::cout << "All tests passed." << std::endl;
    return 0;
}
