#pragma once
#include <vector>
#include <functional>

// Template-based DFS for adjacency list graphs
template<typename T>
void dfs(const std::vector<std::vector<T>>& adj, int start, std::vector<bool>& visited, std::function<void(int)> visit) {
    visited[start] = true;
    visit(start);
    for (const auto& v : adj[start]) {
        if (!visited[v]) dfs(adj, v, visited, visit);
    }
} 