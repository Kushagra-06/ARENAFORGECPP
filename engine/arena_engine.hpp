/*
 ArenaForge - C++ DSA Engine :-
 Modernized C++ module replacing the legacy C algorithms.
 1. Sorting: std::sort (ranking & seeding)
 2. Heap / Priority Q: std::priority_queue (matchmaking)
 3. Tree: knockout bracket (bracket tree) using smart pointers
 4. Hash table: std::unordered_map for team-name -> team-id lookup 
 5. Queue: std::queue for registration / waitlist
 6. Linked list: std::vector or std::list for match history
 7. Graph: Adjacency list using std::vector + greedy colouring
 */
#pragma once

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <queue>

namespace arena {

    constexpr int AE_BYE = -1; // empty bracket slot (team advances automatically)
    constexpr int AE_TBD = -2; // winner not decided yet
    constexpr double AE_REPEAT_PENALTY = 1000.0;

    /* 1. SORTING */
    struct RankEntry {
        int id;       // team id
        double score; // rating / rank key
    };

    // Use std::sort with this comparator instead of custom merge/quick sort
    bool rankEntryComparator(const RankEntry& a, const RankEntry& b);

    /* 6. LINKED LIST (MATCH HISTORY) */
    struct HistNode {
        int match_id, round;
        int team_a, team_b;
        int score_a, score_b;
        int winner;
    };

    class MatchHistory {
    private:
        std::vector<HistNode> history; // Replaces manual linked list
    public:
        void append(const HistNode& rec);
        int meetings(int a, int b) const; // times a met b
        const std::vector<HistNode>& getHistory() const;
        size_t count() const;
    };

    /* 2. HEAP / PRIORITY QUEUE (MATCHMAKING) */
    struct MatchPair {
        int team_a, team_b;
        double cost; // Fair Matchmaking Score: LOWER = better match
        
        bool operator>(const MatchPair& other) const {
            return cost > other.cost; // For std::priority_queue (min-heap)
        }
    };

    // Fair Matchmaking Score = |ratingA - ratingB| + PENALTY * previous meetings
    double calculateFairScore(double rating_a, double rating_b, int prev_meetings);

    // Pairs teams greedily from the cheapest candidate pair upward.
    int matchmake(const std::vector<RankEntry>& teams, const MatchHistory& hist, 
                  std::vector<MatchPair>& out, int max_out);

    /* 3. TREE (BRACKET) */
    struct BracketNode {
        int node_id; // >0 for matches, -1 for leaves
        int round;   // 0 = leaf, 1 = first round, ...
        int team_id; // team id, AE_BYE or AE_TBD
        std::unique_ptr<BracketNode> left;
        std::unique_ptr<BracketNode> right;
        
        BracketNode(int id, int r, int t) : node_id(id), round(r), team_id(t) {}
    };

    class BracketTree {
    private:
        std::unique_ptr<BracketNode> root;
    public:
        void build(const std::vector<int>& seeded_ids);
        void collectReady(std::vector<BracketNode*>& out, int max);
        bool reportWinner(int node_id, int winner);
        int getChampion() const;
        const BracketNode* getRoot() const { return root.get(); }
    };

    /* 4. HASH TABLE */
    class TeamDirectory {
    private:
        std::unordered_map<std::string, int> table; // Replaces manual hashing
    public:
        void put(const std::string& key, int value);
        bool get(const std::string& key, int& out) const;
        bool remove(const std::string& key);
        size_t count() const;
    };

    /* 5. QUEUE */
    class Waitlist {
    private:
        std::queue<int> q; // Replaces manual linked list queue
    public:
        void enqueue(int v);
        bool dequeue(int& out);
        bool peek(int& out) const;
        size_t size() const;
    };

    /* 7. GRAPH */
    class Graph {
    private:
        int n; // number of vertices
        std::vector<std::vector<int>> adj; // adjacency lists replacing raw double pointers
    public:
        explicit Graph(int n);
        bool addEdge(int u, int v); // undirected
        bool hasEdge(int u, int v) const;
        int getDegree(int v) const;
        
        // Welsh-Powell greedy colouring. Returns k (time slots needed).
        int colorGraph(std::vector<int>& colors) const;
        
        // Builds conflicts based on teams shared across matches
        static Graph buildConflicts(const std::vector<int>& team_a, const std::vector<int>& team_b);
    };
}
