#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_set>
#include <functional>

// Simple undirected graph template (adjacency list)
template<typename T>
class Graph {
public:
    std::vector<T> nodes;
    std::vector<std::vector<int>> adj; // adjacency list by index

    void add_node(const T& value) {
        nodes.push_back(value);
        adj.emplace_back();
    }

    void add_edge(int u, int v) {
        if (u < adj.size() && v < adj.size()) {
            adj[u].push_back(v);
            adj[v].push_back(u); // undirected
        }
    }

    // Utility: BFS traversal
    void bfs(int start, std::function<void(const T&)> visit) const {
        if (start >= nodes.size()) return;
        std::vector<bool> visited(nodes.size(), false);
        std::queue<int> q;
        q.push(start);
        visited[start] = true;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            visit(nodes[u]);
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
    }

    // Utility: DFS traversal
    void dfs(int start, std::function<void(const T&)> visit) const {
        if (start >= nodes.size()) return;
        std::vector<bool> visited(nodes.size(), false);
        std::stack<int> s;
        s.push(start);
        while (!s.empty()) {
            int u = s.top(); s.pop();
            if (!visited[u]) {
                visit(nodes[u]);
                visited[u] = true;
                for (auto it = adj[u].rbegin(); it != adj[u].rend(); ++it) {
                    if (!visited[*it]) s.push(*it);
                }
            }
        }
    }

    // Utility: Reverse BFS traversal
    void reverse_bfs(int start, std::function<void(const T&)> visit) const {
        if (start >= nodes.size()) return;
        std::vector<bool> visited(nodes.size(), false);
        std::queue<int> q;
        std::vector<int> order;
        q.push(start);
        visited[start] = true;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            order.push_back(u);
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        for (auto it = order.rbegin(); it != order.rend(); ++it) {
            visit(nodes[*it]);
        }
    }

    // Utility: Reverse DFS traversal
    void reverse_dfs(int start, std::function<void(const T&)> visit) const {
        if (start >= nodes.size()) return;
        std::vector<bool> visited(nodes.size(), false);
        std::vector<int> order;
        std::stack<int> s;
        s.push(start);
        while (!s.empty()) {
            int u = s.top(); s.pop();
            if (!visited[u]) {
                order.push_back(u);
                visited[u] = true;
                for (auto it = adj[u].begin(); it != adj[u].end(); ++it) {
                    if (!visited[*it]) s.push(*it);
                }
            }
        }
        for (auto it = order.rbegin(); it != order.rend(); ++it) {
            visit(nodes[*it]);
        }
    }
}; 