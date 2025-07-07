#pragma once
#include <vector>
#include <queue>
#include <functional>

// Template-based BFS for adjacency list graphs
template<typename T>
void bfs(const std::vector<std::vector<T>>& adj, int start, std::function<void(int)> visit) {
    std::vector<bool> visited(adj.size(), false);
    std::queue<int> q;
    q.push(start);
    visited[start] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        visit(u);
        for (const auto& v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
} 