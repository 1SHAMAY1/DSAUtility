#pragma once
#include <vector>
#include <functional>
#include <stdexcept>

// Template-based Segment Tree for range queries and point updates
template<typename T, typename F = std::function<T(const T&, const T&)>>
class SegmentTree {
    std::vector<T> tree;
    int n;
    F merge;
    T identity;
public:
    SegmentTree(int size, F merge_func, T id) : n(size), merge(merge_func), identity(id) {
        tree.assign(2 * n, identity);
    }
    void build(const std::vector<T>& data) {
        for (int i = 0; i < n; ++i) tree[n + i] = data[i];
        for (int i = n - 1; i > 0; --i) tree[i] = merge(tree[i << 1], tree[i << 1 | 1]);
    }
    void update(int pos, T value) {
        pos += n;
        tree[pos] = value;
        for (pos >>= 1; pos > 0; pos >>= 1)
            tree[pos] = merge(tree[pos << 1], tree[pos << 1 | 1]);
    }
    T query(int l, int r) const { // [l, r)
        T resl = identity, resr = identity;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) resl = merge(resl, tree[l++]);
            if (r & 1) resr = merge(tree[--r], resr);
        }
        return merge(resl, resr);
    }
}; 