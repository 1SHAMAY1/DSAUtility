#pragma once
#include <iostream>
#include <functional>

// Simple binary tree template
// For general trees, more children can be added as needed

template<typename T>
class Tree {
public:
    struct Node {
        T data;
        Node* left;
        Node* right;
        Node(const T& val) : data(val), left(nullptr), right(nullptr) {}
    };

    Node* root;

    Tree() : root(nullptr) {}
    ~Tree() { clear(root); }

    void clear(Node* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

    // Utility: Inorder traversal
    void inorder(std::function<void(const T&)> visit) const { inorder(root, visit); }
    void inorder(Node* node, std::function<void(const T&)> visit) const {
        if (!node) return;
        inorder(node->left, visit);
        visit(node->data);
        inorder(node->right, visit);
    }

    // Utility: Preorder traversal
    void preorder(std::function<void(const T&)> visit) const { preorder(root, visit); }
    void preorder(Node* node, std::function<void(const T&)> visit) const {
        if (!node) return;
        visit(node->data);
        preorder(node->left, visit);
        preorder(node->right, visit);
    }

    // Utility: Postorder traversal
    void postorder(std::function<void(const T&)> visit) const { postorder(root, visit); }
    void postorder(Node* node, std::function<void(const T&)> visit) const {
        if (!node) return;
        postorder(node->left, visit);
        postorder(node->right, visit);
        visit(node->data);
    }

    // Utility: Reverse inorder traversal
    void reverse_inorder(std::function<void(const T&)> visit) const { reverse_inorder(root, visit); }
    void reverse_inorder(Node* node, std::function<void(const T&)> visit) const {
        if (!node) return;
        reverse_inorder(node->right, visit);
        visit(node->data);
        reverse_inorder(node->left, visit);
    }

    // Utility: Reverse preorder traversal
    void reverse_preorder(std::function<void(const T&)> visit) const { reverse_preorder(root, visit); }
    void reverse_preorder(Node* node, std::function<void(const T&)> visit) const {
        if (!node) return;
        visit(node->data);
        reverse_preorder(node->right, visit);
        reverse_preorder(node->left, visit);
    }

    // Utility: Reverse postorder traversal
    void reverse_postorder(std::function<void(const T&)> visit) const { reverse_postorder(root, visit); }
    void reverse_postorder(Node* node, std::function<void(const T&)> visit) const {
        if (!node) return;
        reverse_postorder(node->right, visit);
        reverse_postorder(node->left, visit);
        visit(node->data);
    }
}; 