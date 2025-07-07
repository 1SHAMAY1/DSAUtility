#pragma once
#include <unordered_set>

// Template-based LCA for binary trees with parent pointers
template<typename Node>
Node* lca(Node* a, Node* b) {
    std::unordered_set<Node*> ancestors;
    while (a) {
        ancestors.insert(a);
        a = a->parent;
    }
    while (b) {
        if (ancestors.count(b)) return b;
        b = b->parent;
    }
    return nullptr;
} 