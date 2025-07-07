#pragma once
#include <vector>
#include <numeric>

// Template-based Disjoint Set (Union-Find) for integer elements
class DisjointSet {
    std::vector<int> parent, rank;
public:
    explicit DisjointSet(int n) : parent(n), rank(n, 0) {
        std::iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    void unite(int x, int y) {
        int xr = find(x), yr = find(y);
        if (xr == yr) return;
        if (rank[xr] < rank[yr]) parent[xr] = yr;
        else if (rank[xr] > rank[yr]) parent[yr] = xr;
        else { parent[yr] = xr; rank[xr]++; }
    }
    bool connected(int x, int y) { return find(x) == find(y); }
}; 