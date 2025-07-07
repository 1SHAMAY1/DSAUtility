#pragma once
#include <vector>
#include <stdexcept>

// Template-based Fenwick Tree (Binary Indexed Tree) for prefix sums/queries
template<typename T>
class FenwickTree {
    std::vector<T> bit;
    int n;
public:
    FenwickTree(int size) : n(size) { bit.assign(n + 1, T{}); }
    void update(int idx, T delta) {
        for (++idx; idx <= n; idx += idx & -idx) bit[idx] += delta;
    }
    T query(int idx) const { // sum [0, idx]
        T res{};
        for (++idx; idx > 0; idx -= idx & -idx) res += bit[idx];
        return res;
    }
    T range_query(int l, int r) const { // sum [l, r]
        return query(r) - query(l - 1);
    }
}; 