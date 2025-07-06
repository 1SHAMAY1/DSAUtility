#pragma once
#include <memory>
#include <stdexcept>
#include <iostream>
#include <initializer_list>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include "../Utils/Print.hpp"
#include "../Utils/Logger.hpp"

namespace dsa {
namespace structures {

/**
 * @brief AVL Tree Node structure
 */
template<typename T>
struct AVLNode {
    T data;
    std::unique_ptr<AVLNode<T>> left;
    std::unique_ptr<AVLNode<T>> right;
    int height;
    
    AVLNode(const T& value) : data(value), left(nullptr), right(nullptr), height(1) {}
    AVLNode(T&& value) : data(std::move(value)), left(nullptr), right(nullptr), height(1) {}
};

/**
 * @brief AVL Tree Data Structure
 */
template<typename T>
class AVLTree {
private:
    std::unique_ptr<AVLNode<T>> root;
    size_t size_;

public:
    // Constructors
    AVLTree() : root(nullptr), size_(0) {}
    
    AVLTree(std::initializer_list<T> init_list) : root(nullptr), size_(0) {
        for (const auto& item : init_list) {
            insert(item);
        }
    }
    
    AVLTree(const AVLTree& other) : root(nullptr), size_(0) {
        copy_tree(other.root.get());
    }
    
    AVLTree(AVLTree&& other) noexcept 
        : root(std::move(other.root)), size_(other.size_) {
        other.size_ = 0;
    }
    
    // Destructor
    ~AVLTree() = default;
    
    // Assignment operators
    AVLTree& operator=(const AVLTree& other) {
        if (this != &other) {
            clear();
            copy_tree(other.root.get());
        }
        return *this;
    }
    
    AVLTree& operator=(AVLTree&& other) noexcept {
        if (this != &other) {
            root = std::move(other.root);
            size_ = other.size_;
            other.size_ = 0;
        }
        return *this;
    }
    
    // Accessors
    bool empty() const { return size_ == 0; }
    size_t size() const { return size_; }
    
    // Modifiers
    void insert(const T& value) {
        root = insert_recursive(std::move(root), value);
        size_++;
    }
    
    void insert(T&& value) {
        root = insert_recursive(std::move(root), std::move(value));
        size_++;
    }
    
    void remove(const T& value) {
        if (contains(value)) {
            root = remove_recursive(std::move(root), value);
            size_--;
        }
    }
    
    void clear() {
        root.reset();
        size_ = 0;
    }
    
    // Search operations
    bool contains(const T& value) const {
        return find_node(root.get(), value) != nullptr;
    }
    
    T* find(const T& value) {
        AVLNode<T>* node = find_node(root.get(), value);
        return node ? &node->data : nullptr;
    }
    
    const T* find(const T& value) const {
        AVLNode<T>* node = find_node(root.get(), value);
        return node ? &node->data : nullptr;
    }
    
    // Tree traversal
    std::vector<T> inorder() const {
        std::vector<T> result;
        inorder_recursive(root.get(), result);
        return result;
    }
    
    std::vector<T> preorder() const {
        std::vector<T> result;
        preorder_recursive(root.get(), result);
        return result;
    }
    
    std::vector<T> postorder() const {
        std::vector<T> result;
        postorder_recursive(root.get(), result);
        return result;
    }
    
    std::vector<T> levelorder() const {
        std::vector<T> result;
        if (!root) return result;
        
        std::queue<AVLNode<T>*> q;
        q.push(root.get());
        
        while (!q.empty()) {
            AVLNode<T>* current = q.front();
            q.pop();
            result.push_back(current->data);
            
            if (current->left) {
                q.push(current->left.get());
            }
            if (current->right) {
                q.push(current->right.get());
            }
        }
        return result;
    }
    
    // Tree properties
    size_t height() const {
        return height_recursive(root.get());
    }
    
    size_t depth(const T& value) const {
        AVLNode<T>* node = find_node(root.get(), value);
        if (!node) return static_cast<size_t>(-1);
        
        size_t depth = 0;
        AVLNode<T>* current = root.get();
        while (current && current != node) {
            if (value < current->data) {
                current = current->left.get();
            } else {
                current = current->right.get();
            }
            depth++;
        }
        return depth;
    }
    
    bool is_balanced() const {
        return is_balanced_recursive(root.get()) != -1;
    }
    
    // AVL-specific operations
    void balance() {
        // AVL trees are always balanced, so this is a no-op
        // But we can rebalance if needed
        std::vector<T> elements = inorder();
        clear();
        for (const auto& element : elements) {
            insert(element);
        }
    }
    
    // Utility functions
    void print() const {
        print_recursive(root.get(), "", true);
    }
    
    void print_inorder() const {
        std::vector<T> elements = inorder();
        utils::Print::printVectorWithPrefix(elements, "Inorder: ");
        std::cout << std::endl;
    }
    
    void print_preorder() const {
        std::vector<T> elements = preorder();
        utils::Print::printVectorWithPrefix(elements, "Preorder: ");
        std::cout << std::endl;
    }
    
    void print_postorder() const {
        std::vector<T> elements = postorder();
        utils::Print::printVectorWithPrefix(elements, "Postorder: ");
        std::cout << std::endl;
    }
    
    void print_levelorder() const {
        std::vector<T> elements = levelorder();
        utils::Print::printVectorWithPrefix(elements, "Levelorder: ");
        std::cout << std::endl;
    }
    
    std::string to_string() const {
        std::stringstream ss;
        ss << "AVLTree[size=" << size_ << ", height=" << height() << ", balanced=" << is_balanced() << "]";
        return ss.str();
    }
    
    // Iterator support
    class Iterator {
    private:
        std::vector<T> elements;
        size_t current_index;
        
    public:
        Iterator(const AVLTree<T>& tree, bool end = false) 
            : elements(tree.inorder()), current_index(end ? elements.size() : 0) {}
        
        T& operator*() { return elements[current_index]; }
        const T& operator*() const { return elements[current_index]; }
        
        Iterator& operator++() {
            ++current_index;
            return *this;
        }
        
        Iterator operator++(int) {
            Iterator temp = *this;
            ++current_index;
            return temp;
        }
        
        bool operator==(const Iterator& other) const {
            return current_index == other.current_index;
        }
        
        bool operator!=(const Iterator& other) const {
            return current_index != other.current_index;
        }
    };
    
    Iterator begin() { return Iterator(*this); }
    Iterator end() { return Iterator(*this, true); }
    Iterator begin() const { return Iterator(*this); }
    Iterator end() const { return Iterator(*this, true); }

private:
    // Helper functions
    AVLNode<T>* find_min(AVLNode<T>* node) const {
        if (!node || !node->left) return node;
        return find_min(node->left.get());
    }
    
    AVLNode<T>* find_max(AVLNode<T>* node) const {
        if (!node || !node->right) return node;
        return find_max(node->right.get());
    }
    
    AVLNode<T>* find_node(AVLNode<T>* node, const T& value) const {
        if (!node || node->data == value) return node;
        if (value < node->data) {
            return find_node(node->left.get(), value);
        } else {
            return find_node(node->right.get(), value);
        }
    }
    
    std::unique_ptr<AVLNode<T>> insert_recursive(std::unique_ptr<AVLNode<T>> node, const T& value) {
        if (!node) {
            return std::make_unique<AVLNode<T>>(value);
        }
        
        if (value < node->data) {
            node->left = insert_recursive(std::move(node->left), value);
        } else if (value > node->data) {
            node->right = insert_recursive(std::move(node->right), value);
        } else {
            return node; // Duplicate values not allowed
        }
        
        // Update height
        node->height = 1 + std::max(get_height(node->left.get()), get_height(node->right.get()));
        
        // Get balance factor
        int balance = get_balance(node.get());
        
        // Left Left Case
        if (balance > 1 && value < node->left->data) {
            return right_rotate(std::move(node));
        }
        
        // Right Right Case
        if (balance < -1 && value > node->right->data) {
            return left_rotate(std::move(node));
        }
        
        // Left Right Case
        if (balance > 1 && value > node->left->data) {
            node->left = left_rotate(std::move(node->left));
            return right_rotate(std::move(node));
        }
        
        // Right Left Case
        if (balance < -1 && value < node->right->data) {
            node->right = right_rotate(std::move(node->right));
            return left_rotate(std::move(node));
        }
        
        return node;
    }
    
    std::unique_ptr<AVLNode<T>> insert_recursive(std::unique_ptr<AVLNode<T>> node, T&& value) {
        if (!node) {
            return std::make_unique<AVLNode<T>>(std::move(value));
        }
        
        if (value < node->data) {
            node->left = insert_recursive(std::move(node->left), std::move(value));
        } else if (value > node->data) {
            node->right = insert_recursive(std::move(node->right), std::move(value));
        } else {
            return node; // Duplicate values not allowed
        }
        
        // Update height
        node->height = 1 + std::max(get_height(node->left.get()), get_height(node->right.get()));
        
        // Get balance factor
        int balance = get_balance(node.get());
        
        // Left Left Case
        if (balance > 1 && value < node->left->data) {
            return right_rotate(std::move(node));
        }
        
        // Right Right Case
        if (balance < -1 && value > node->right->data) {
            return left_rotate(std::move(node));
        }
        
        // Left Right Case
        if (balance > 1 && value > node->left->data) {
            node->left = left_rotate(std::move(node->left));
            return right_rotate(std::move(node));
        }
        
        // Right Left Case
        if (balance < -1 && value < node->right->data) {
            node->right = right_rotate(std::move(node->right));
            return left_rotate(std::move(node));
        }
        
        return node;
    }
    
    std::unique_ptr<AVLNode<T>> remove_recursive(std::unique_ptr<AVLNode<T>> node, const T& value) {
        if (!node) return nullptr;
        
        if (value < node->data) {
            node->left = remove_recursive(std::move(node->left), value);
        } else if (value > node->data) {
            node->right = remove_recursive(std::move(node->right), value);
        } else {
            // Node to delete found
            if (!node->left) {
                return std::move(node->right);
            } else if (!node->right) {
                return std::move(node->left);
            } else {
                // Node has two children
                AVLNode<T>* min_right = find_min(node->right.get());
                node->data = min_right->data;
                node->right = remove_recursive(std::move(node->right), min_right->data);
            }
        }
        
        if (!node) return nullptr;
        
        // Update height
        node->height = 1 + std::max(get_height(node->left.get()), get_height(node->right.get()));
        
        // Get balance factor
        int balance = get_balance(node.get());
        
        // Left Left Case
        if (balance > 1 && get_balance(node->left.get()) >= 0) {
            return right_rotate(std::move(node));
        }
        
        // Left Right Case
        if (balance > 1 && get_balance(node->left.get()) < 0) {
            node->left = left_rotate(std::move(node->left));
            return right_rotate(std::move(node));
        }
        
        // Right Right Case
        if (balance < -1 && get_balance(node->right.get()) <= 0) {
            return left_rotate(std::move(node));
        }
        
        // Right Left Case
        if (balance < -1 && get_balance(node->right.get()) > 0) {
            node->right = right_rotate(std::move(node->right));
            return left_rotate(std::move(node));
        }
        
        return node;
    }
    
    void inorder_recursive(AVLNode<T>* node, std::vector<T>& result) const {
        if (node) {
            inorder_recursive(node->left.get(), result);
            result.push_back(node->data);
            inorder_recursive(node->right.get(), result);
        }
    }
    
    void preorder_recursive(AVLNode<T>* node, std::vector<T>& result) const {
        if (node) {
            result.push_back(node->data);
            preorder_recursive(node->left.get(), result);
            preorder_recursive(node->right.get(), result);
        }
    }
    
    void postorder_recursive(AVLNode<T>* node, std::vector<T>& result) const {
        if (node) {
            postorder_recursive(node->left.get(), result);
            postorder_recursive(node->right.get(), result);
            result.push_back(node->data);
        }
    }
    
    size_t height_recursive(AVLNode<T>* node) const {
        if (!node) return 0;
        return 1 + std::max(height_recursive(node->left.get()), height_recursive(node->right.get()));
    }
    
    int is_balanced_recursive(AVLNode<T>* node) const {
        if (!node) return 0;
        
        int left_height = is_balanced_recursive(node->left.get());
        if (left_height == -1) return -1;
        
        int right_height = is_balanced_recursive(node->right.get());
        if (right_height == -1) return -1;
        
        if (std::abs(left_height - right_height) > 1) return -1;
        
        return 1 + std::max(left_height, right_height);
    }
    
    int get_height(AVLNode<T>* node) const {
        return node ? node->height : 0;
    }
    
    int get_balance(AVLNode<T>* node) const {
        return node ? get_height(node->left.get()) - get_height(node->right.get()) : 0;
    }
    
    std::unique_ptr<AVLNode<T>> right_rotate(std::unique_ptr<AVLNode<T>> y) {
        std::unique_ptr<AVLNode<T>> x = std::move(y->left);
        y->left = std::move(x->right);
        x->right = std::move(y);
        
        // Update heights
        y->height = 1 + std::max(get_height(y->left.get()), get_height(y->right.get()));
        x->height = 1 + std::max(get_height(x->left.get()), get_height(x->right.get()));
        
        return x;
    }
    
    std::unique_ptr<AVLNode<T>> left_rotate(std::unique_ptr<AVLNode<T>> x) {
        std::unique_ptr<AVLNode<T>> y = std::move(x->right);
        x->right = std::move(y->left);
        y->left = std::move(x);
        
        // Update heights
        x->height = 1 + std::max(get_height(x->left.get()), get_height(x->right.get()));
        y->height = 1 + std::max(get_height(y->left.get()), get_height(y->right.get()));
        
        return y;
    }
    
    void copy_tree(AVLNode<T>* node) {
        if (node) {
            insert(node->data);
            copy_tree(node->left.get());
            copy_tree(node->right.get());
        }
    }
    
    void print_recursive(AVLNode<T>* node, const std::string& prefix, bool is_left) const {
        if (node) {
            std::cout << prefix;
            std::cout << (is_left ? "├── " : "└── ");
            std::cout << node->data << " (h:" << node->height << ")" << std::endl;
            
            print_recursive(node->left.get(), prefix + (is_left ? "│   " : "    "), true);
            print_recursive(node->right.get(), prefix + (is_left ? "│   " : "    "), false);
        }
    }
};

// Type aliases for common use cases
using IntAVLTree = AVLTree<int>;
using DoubleAVLTree = AVLTree<double>;
using StringAVLTree = AVLTree<std::string>;

} // namespace structures
} // namespace dsa
