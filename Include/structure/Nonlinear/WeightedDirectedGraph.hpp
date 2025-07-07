#pragma once
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <functional>
#include <limits>

// Template-based weighted, directed graph
// Nodes are indexed by int, edges have weights of type W
template<typename T, typename W = int>
class WeightedDirectedGraph {
public:
    std::vector<T> nodes;
    std::vector<std::unordered_map<int, W>> adj; // adj[u][v] = weight

    void add_node(const T& value) {
        nodes.push_back(value);
        adj.emplace_back();
    }
    void add_edge(int u, int v, W weight) {
        if (u < adj.size() && v < adj.size()) {
            adj[u][v] = weight;
        }
    }
    // Utility: BFS traversal
    void bfs(int start, std::function<void(int, const T&)> visit) const {
        if (start >= nodes.size()) return;
        std::vector<bool> visited(nodes.size(), false);
        std::queue<int> q;
        q.push(start);
        visited[start] = true;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            visit(u, nodes[u]);
            for (const auto& [v, w] : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
    }
    // Utility: DFS traversal
    void dfs(int start, std::function<void(int, const T&)> visit) const {
        if (start >= nodes.size()) return;
        std::vector<bool> visited(nodes.size(), false);
        std::stack<int> s;
        s.push(start);
        while (!s.empty()) {
            int u = s.top(); s.pop();
            if (!visited[u]) {
                visit(u, nodes[u]);
                visited[u] = true;
                for (auto it = adj[u].rbegin(); it != adj[u].rend(); ++it) {
                    if (!visited[it->first]) s.push(it->first);
                }
            }
        }
    }
    // Utility: Dijkstra's shortest path
    std::vector<W> dijkstra(int src) const {
        std::vector<W> dist(nodes.size(), std::numeric_limits<W>::max());
        dist[src] = 0;
        using P = std::pair<W, int>;
        std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
        pq.emplace(0, src);
        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d > dist[u]) continue;
            for (const auto& [v, w] : adj[u]) {
                if (dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    pq.emplace(dist[v], v);
                }
            }
        }
        return dist;
    }
}; 