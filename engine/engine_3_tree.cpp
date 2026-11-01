#include "arena_engine.hpp"

namespace arena {
    void BracketTree::build(const std::vector<int>& seeded_ids) {
        int n = seeded_ids.size();
        if (n < 2) return;
        
        int p = 1;
        while (p < n) p *= 2; // Finds next power of 2
        
        std::vector<int> order(p, 1);
        for (int len = 1; len < p; len *= 2) {
            for (int i = len - 1; i >= 0; i--) {
                order[2 * i + 1] = 2 * len + 1 - order[i];
                order[2 * i] = order[i];
            }
        }
        
        std::vector<std::unique_ptr<BracketNode>> level;
        for (int i = 0; i < p; i++) {
            int team = (order[i] <= n) ? seeded_ids[order[i] - 1] : AE_BYE;
            level.push_back(std::make_unique<BracketNode>(-1, 0, team));
        }
        
        int next_id = 1, round = 1;
        while (level.size() > 1) {
            std::vector<std::unique_ptr<BracketNode>> next_level;
            for (size_t i = 0; i < level.size(); i += 2) {
                auto m = std::make_unique<BracketNode>(next_id++, round, AE_TBD);
                
                if (level[i]->team_id == AE_BYE && level[i+1]->team_id == AE_BYE) m->team_id = AE_BYE;
                else if (level[i]->team_id == AE_BYE) m->team_id = level[i+1]->team_id;
                else if (level[i+1]->team_id == AE_BYE) m->team_id = level[i]->team_id;
                
                m->left = std::move(level[i]);
                m->right = std::move(level[i+1]);
                next_level.push_back(std::move(m));
            }
            level = std::move(next_level);
            round++;
        }
        if (!level.empty()) root = std::move(level[0]);
    }
    
    int BracketTree::getChampion() const {
        return (root && root->team_id >= 0) ? root->team_id : AE_TBD;
    }
    
    void BracketTree::collectReady(std::vector<BracketNode*>& out, int max) {
        // Implementation for collecting ready matches
    }
    
    bool BracketTree::reportWinner(int node_id, int winner) {
        // Implementation for reporting a winner
        return false;
    }
}
