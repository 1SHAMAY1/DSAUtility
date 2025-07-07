#pragma once
#include <vector>
#include <queue>
#include <limits>
#include <utility>

// Template-based Dijkstra's algorithm for weighted graphs (adjacency list)
template<typename W>
std::vector<W> dijkstra(const std::vector<std::vector<std::pair<int, W>>>& adj, int src) {
    const W INF = std::numeric_limits<W>::max();
    std::vector<W> dist(adj.size(), INF);
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