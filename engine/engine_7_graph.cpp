#include "arena_engine.hpp"
#include <algorithm>
#include <numeric>

namespace arena {
    Graph::Graph(int n) : n(n), adj(n) {}

    bool Graph::addEdge(int u, int v) {
        if (u < 0 || v < 0 || u >= n || v >= n || u == v) return false;
        if (!hasEdge(u, v)) {
            adj[u].push_back(v);
            adj[v].push_back(u); // Ensures graph is undirected
        }
        return true;
    }

    bool Graph::hasEdge(int u, int v) const {
        if (u < 0 || v < 0 || u >= n || v >= n) return false;
        return std::find(adj[u].begin(), adj[u].end(), v) != adj[u].end();
    }

    int Graph::getDegree(int v) const {
        if (v < 0 || v >= n) return 0;
        return adj[v].size();
    }

    int Graph::colorGraph(std::vector<int>& colors) const {
        if (n == 0) return 0;
        colors.assign(n, -1);
        
        std::vector<int> order(n);
        std::iota(order.begin(), order.end(), 0);
        std::sort(order.begin(), order.end(), [&](int a, int b) {
            return getDegree(a) > getDegree(b); // Sort by degree descending
        });

        int used_colors = 0;
        for (int v : order) {
            std::vector<bool> forbidden(n, false);
            for (int neighbor : adj[v]) {
                if (colors[neighbor] >= 0) forbidden[colors[neighbor]] = true;
            }
            int c = 0;
            while (c < n && forbidden[c]) c++;
            colors[v] = c;
            used_colors = std::max(used_colors, c + 1);
        }
        return used_colors;
    }

    Graph Graph::buildConflicts(const std::vector<int>& team_a, const std::vector<int>& team_b) {
        int m = team_a.size();
        Graph g(m);
        for (int i = 0; i < m; i++) {
            for (int j = i + 1; j < m; j++) {
                if (team_a[i] == team_a[j] || team_a[i] == team_b[j] || 
                    team_b[i] == team_a[j] || team_b[i] == team_b[j]) {
                    g.addEdge(i, j);
                }
            }
        }
        return g;
    }
}
