#pragma once
#include "Tree.hpp"
#include <algorithm>
#include <functional>

// Template-based AVLTree as a child of Tree<T>
template<typename T>
class AVLTree : public Tree<T> {
    using Node = typename Tree<T>::Node;
public:
    void insert(const T& value) {
        this->root = insertRec(this->root, value);
    }
    void remove(const T& value) {
        this->root = removeRec(this->root, value);
    }
private:
    int height(Node* node) const {
        if (!node) return 0;
        return 1 + std::max(height(node->left), height(node->right));
    }
    int balanceFactor(Node* node) const {
        return node ? height(node->left) - height(node->right) : 0;
    }
    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        return x;
    }
    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        return y;
    }
    Node* insertRec(Node* node, const T& value) {
        if (!node) return new Node(value);
        if (value < node->data)
            node->left = insertRec(node->left, value);
        else if (value > node->data)
            node->right = insertRec(node->right, value);
        else
            return node; // no duplicates
        int bf = balanceFactor(node);
        // Left Left
        if (bf > 1 && value < node->left->data)
            return rightRotate(node);
        // Right Right
        if (bf < -1 && value > node->right->data)
            return leftRotate(node);
        // Left Right
        if (bf > 1 && value > node->left->data) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        // Right Left
        if (bf < -1 && value < node->right->data) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }
    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current && current->left)
            current = current->left;
        return current;
    }
    Node* removeRec(Node* node, const T& value) {
        if (!node) return node;
        if (value < node->data)
            node->left = removeRec(node->left, value);
        else if (value > node->data)
            node->right = removeRec(node->right, value);
        else {
            if (!node->left || !node->right) {
                Node* temp = node->left ? node->left : node->right;
                if (!temp) {
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }
                delete temp;
            } else {
                Node* temp = minValueNode(node->right);
                node->data = temp->data;
                node->right = removeRec(node->right, temp->data);
            }
        }
        if (!node) return node;
        int bf = balanceFactor(node);
        // Left Left
        if (bf > 1 && balanceFactor(node->left) >= 0)
            return rightRotate(node);
        // Left Right
        if (bf > 1 && balanceFactor(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        // Right Right
        if (bf < -1 && balanceFactor(node->right) <= 0)
            return leftRotate(node);
        // Right Left
        if (bf < -1 && balanceFactor(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }
}; 