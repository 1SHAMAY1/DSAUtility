#pragma once
#include <vector>
#include <memory>
#include <functional>

// Template-based general n-ary tree
// Each node can have any number of children
template<typename T>
class NaryTree {
public:
    struct Node {
        T data;
        std::vector<std::unique_ptr<Node>> children;
        Node(const T& val) : data(val) {}
    };
    std::unique_ptr<Node> root;
    NaryTree() : root(nullptr) {}

    // Insert as child of parent (by pointer)
    Node* insert(Node* parent, const T& value) {
        auto newNode = std::make_unique<Node>(value);
        Node* ptr = newNode.get();
        if (!parent) {
            root = std::move(newNode);
            return root.get();
        } else {
            parent->children.push_back(std::move(newNode));
            return ptr;
        }
    }

    // Preorder traversal
    void traverse(std::function<void(const T&)> visit) const { traverse(root.get(), visit); }
    void traverse(Node* node, std::function<void(const T&)> visit) const {
        if (!node) return;
        visit(node->data);
        for (const auto& child : node->children) traverse(child.get(), visit);
    }
    // Reverse preorder traversal
    void reverse_traverse(std::function<void(const T&)> visit) const { reverse_traverse(root.get(), visit); }
    void reverse_traverse(Node* node, std::function<void(const T&)> visit) const {
        if (!node) return;
        visit(node->data);
        for (auto it = node->children.rbegin(); it != node->children.rend(); ++it)
            reverse_traverse(it->get(), visit);
    }
}; 