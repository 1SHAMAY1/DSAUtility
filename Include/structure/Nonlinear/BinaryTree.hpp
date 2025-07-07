#pragma once
#include "Tree.hpp"
#include <queue>
#include <functional>

// Template-based BinaryTree as a child of Tree<T>
template<typename T>
class BinaryTree : public Tree<T> {
    using Node = typename Tree<T>::Node;
public:
    // Insert as a binary tree (not BST): fill level order
    void insert(const T& value) {
        Node* newNode = new Node(value);
        if (!this->root) {
            this->root = newNode;
            return;
        }
        std::queue<Node*> q;
        q.push(this->root);
        while (!q.empty()) {
            Node* curr = q.front(); q.pop();
            if (!curr->left) {
                curr->left = newNode;
                return;
            } else {
                q.push(curr->left);
            }
            if (!curr->right) {
                curr->right = newNode;
                return;
            } else {
                q.push(curr->right);
            }
        }
    }

    // Remove all nodes with a given value (simple version)
    void remove(const T& value) {
        removeHelper(this->root, value);
    }
private:
    void removeHelper(Node*& node, const T& value) {
        if (!node) return;
        removeHelper(node->left, value);
        removeHelper(node->right, value);
        if (node->data == value) {
            delete node;
            node = nullptr;
        }
    }
}; 