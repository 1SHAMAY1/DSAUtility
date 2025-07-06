#pragma once
#include <memory>
#include <stdexcept>
#include <iostream>
#include <initializer_list>
#include <vector>
#include <queue>
#include "../Utils/Print.hpp"
#include "../Utils/Logger.hpp"

namespace dsa {
namespace structures {

/**
 * @brief Node structure for Binary Search Tree
 */
template<typename T>
struct BSTNode {
    T data;
    std::unique_ptr<BSTNode<T>> left;
    std::unique_ptr<BSTNode<T>> right;
    BSTNode<T>* parent;
    
    BSTNode(const T& value) : data(value), left(nullptr), right(nullptr), parent(nullptr) {}
    BSTNode(T&& value) : data(std::move(value)), left(nullptr), right(nullptr), parent(nullptr) {}
};

/**
 * @brief Binary Search Tree Data Structure
 */
template<typename T>
class BinarySearchTree {
private:
    std::unique_ptr<BSTNode<T>> root;
    size_t size_;

public:
    // Constructors
    BinarySearchTree() : root(nullptr), size_(0) {}
    
    BinarySearchTree(std::initializer_list<T> init_list) : root(nullptr), size_(0) {
        for (const auto& item : init_list) {
            insert(item);
        }
    }
    
    BinarySearchTree(const BinarySearchTree& other) : root(nullptr), size_(0) {
        copy_tree(other.root.get());
    }
    
    BinarySearchTree(BinarySearchTree&& other) noexcept 
        : root(std::move(other.root)), size_(other.size_) {
        other.size_ = 0;
    }
    
    // Destructor
    ~BinarySearchTree() = default;
    
    // Assignment operators
    BinarySearchTree& operator=(const BinarySearchTree& other) {
        if (this != &other) {
            clear();
            copy_tree(other.root.get());
        }
        return *this;
    }
    
    BinarySearchTree& operator=(BinarySearchTree&& other) noexcept {
        if (this != &other) {
            clear();
            root = std::move(other.root);
            size_ = other.size_;
            other.size_ = 0;
        }
        return *this;
    }
    
    // Capacity
    bool empty() const { return size_ == 0; }
    size_t size() const { return size_; }
    
    // Element access
    T& min() {
        if (empty()) {
            throw std::out_of_range("Tree is empty");
        }
        return find_min(root.get())->data;
    }
    
    const T& min() const {
        if (empty()) {
            throw std::out_of_range("Tree is empty");
        }
        return find_min(root.get())->data;
    }
    
    T& max() {
        if (empty()) {
            throw std::out_of_range("Tree is empty");
        }
        return find_max(root.get())->data;
    }
    
    const T& max() const {
        if (empty()) {
            throw std::out_of_range("Tree is empty");
        }
        return find_max(root.get())->data;
    }
    
    // Modifiers
    void clear() {
        root.reset();
        size_ = 0;
    }
    
    void insert(const T& value) {
        root = insert_recursive(std::move(root), value);
        size_++;
    }
    
    void insert(T&& value) {
        root = insert_recursive(std::move(root), std::move(value));
        size_++;
    }
    
    template<typename... Args>
    void emplace(Args&&... args) {
        T value(std::forward<Args>(args)...);
        insert(std::move(value));
    }
    
    void remove(const T& value) {
        if (contains(value)) {
            root = remove_recursive(std::move(root), value);
            size_--;
        }
    }
    
    // Search operations
    bool contains(const T& value) const {
        return find_node(root.get(), value) != nullptr;
    }
    
    T* find(const T& value) {
        BSTNode<T>* node = find_node(root.get(), value);
        return node ? &node->data : nullptr;
    }
    
    const T* find(const T& value) const {
        BSTNode<T>* node = find_node(root.get(), value);
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
        
        std::queue<BSTNode<T>*> q;
        q.push(root.get());
        
        while (!q.empty()) {
            BSTNode<T>* current = q.front();
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
        BSTNode<T>* node = find_node(root.get(), value);
        if (!node) return static_cast<size_t>(-1);
        
        size_t depth = 0;
        BSTNode<T>* current = root.get();
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
    
    bool is_complete() const {
        if (!root) return true;
        
        std::queue<BSTNode<T>*> q;
        q.push(root.get());
        bool found_null = false;
        
        while (!q.empty()) {
            BSTNode<T>* current = q.front();
            q.pop();
            
            if (!current) {
                found_null = true;
            } else {
                if (found_null) return false;
                q.push(current->left.get());
                q.push(current->right.get());
            }
        }
        return true;
    }
    
    bool is_full() const {
        return is_full_recursive(root.get());
    }
    
    bool is_perfect() const {
        size_t h = height();
        return size() == (1ULL << h) - 1;
    }
    
    // Tree operations
    void balance() {
        std::vector<T> elements = inorder();
        clear();
        build_balanced_tree(elements, 0, elements.size() - 1);
    }
    
    void mirror() {
        mirror_recursive(root.get());
    }
    
    BinarySearchTree<T> get_left_subtree() const {
        BinarySearchTree<T> subtree;
        if (root && root->left) {
            subtree.copy_tree(root->left.get());
        }
        return subtree;
    }
    
    BinarySearchTree<T> get_right_subtree() const {
        BinarySearchTree<T> subtree;
        if (root && root->right) {
            subtree.copy_tree(root->right.get());
        }
        return subtree;
    }
    
    // Mathematical operations (for numeric types)
    template<typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, T>::type sum() const {
        std::vector<T> elements = inorder();
        T result = T{};
        for (const auto& item : elements) {
            result += item;
        }
        return result;
    }
    
    template<typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, double>::type average() const {
        if (empty()) {
            throw std::out_of_range("Tree is empty");
        }
        return static_cast<double>(sum()) / size();
    }
    
    // Comparison operators
    bool operator==(const BinarySearchTree& other) const {
        if (size_ != other.size_) return false;
        std::vector<T> this_inorder = inorder();
        std::vector<T> other_inorder = other.inorder();
        return this_inorder == other_inorder;
    }
    
    bool operator!=(const BinarySearchTree& other) const {
        return !(*this == other);
    }
    
    // Utility functions
    void print() const {
        print_recursive(root.get(), "", true);
    }
    
    void print_inorder() const {
        std::vector<T> elements = inorder();
        utils::Print::printVector(elements, "Inorder: ");
        std::cout << std::endl;
    }
    
    void print_preorder() const {
        std::vector<T> elements = preorder();
        utils::Print::printVector(elements, "Preorder: ");
        std::cout << std::endl;
    }
    
    void print_postorder() const {
        std::vector<T> elements = postorder();
        utils::Print::printVector(elements, "Postorder: ");
        std::cout << std::endl;
    }
    
    void print_levelorder() const {
        std::vector<T> elements = levelorder();
        utils::Print::printVector(elements, "Levelorder: ");
        std::cout << std::endl;
    }
    
    std::string to_string() const {
        std::stringstream ss;
        ss << "BinarySearchTree[size=" << size_ << ", height=" << height() << "]";
        return ss.str();
    }
    
    // Iterator support (basic implementation)
    class Iterator {
    private:
        std::vector<T> elements;
        size_t current_index;
        
    public:
        Iterator(const BinarySearchTree<T>& tree, bool end = false) 
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
    BSTNode<T>* find_min(BSTNode<T>* node) const {
        if (!node || !node->left) return node;
        return find_min(node->left.get());
    }
    
    BSTNode<T>* find_max(BSTNode<T>* node) const {
        if (!node || !node->right) return node;
        return find_max(node->right.get());
    }
    
    BSTNode<T>* find_node(BSTNode<T>* node, const T& value) const {
        if (!node || node->data == value) return node;
        if (value < node->data) {
            return find_node(node->left.get(), value);
        } else {
            return find_node(node->right.get(), value);
        }
    }
    
    std::unique_ptr<BSTNode<T>> insert_recursive(std::unique_ptr<BSTNode<T>> node, const T& value) {
        if (!node) {
            return std::make_unique<BSTNode<T>>(value);
        }
        
        if (value < node->data) {
            node->left = insert_recursive(std::move(node->left), value);
        } else if (value > node->data) {
            node->right = insert_recursive(std::move(node->right), value);
        }
        
        return node;
    }
    
    std::unique_ptr<BSTNode<T>> insert_recursive(std::unique_ptr<BSTNode<T>> node, T&& value) {
        if (!node) {
            return std::make_unique<BSTNode<T>>(std::move(value));
        }
        
        if (value < node->data) {
            node->left = insert_recursive(std::move(node->left), std::move(value));
        } else if (value > node->data) {
            node->right = insert_recursive(std::move(node->right), std::move(value));
        }
        
        return node;
    }
    
    std::unique_ptr<BSTNode<T>> remove_recursive(std::unique_ptr<BSTNode<T>> node, const T& value) {
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
                BSTNode<T>* min_right = find_min(node->right.get());
                node->data = min_right->data;
                node->right = remove_recursive(std::move(node->right), min_right->data);
            }
        }
        
        return node;
    }
    
    void inorder_recursive(BSTNode<T>* node, std::vector<T>& result) const {
        if (node) {
            inorder_recursive(node->left.get(), result);
            result.push_back(node->data);
            inorder_recursive(node->right.get(), result);
        }
    }
    
    void preorder_recursive(BSTNode<T>* node, std::vector<T>& result) const {
        if (node) {
            result.push_back(node->data);
            preorder_recursive(node->left.get(), result);
            preorder_recursive(node->right.get(), result);
        }
    }
    
    void postorder_recursive(BSTNode<T>* node, std::vector<T>& result) const {
        if (node) {
            postorder_recursive(node->left.get(), result);
            postorder_recursive(node->right.get(), result);
            result.push_back(node->data);
        }
    }
    
    size_t height_recursive(BSTNode<T>* node) const {
        if (!node) return 0;
        return 1 + std::max(height_recursive(node->left.get()), height_recursive(node->right.get()));
    }
    
    int is_balanced_recursive(BSTNode<T>* node) const {
        if (!node) return 0;
        
        int left_height = is_balanced_recursive(node->left.get());
        if (left_height == -1) return -1;
        
        int right_height = is_balanced_recursive(node->right.get());
        if (right_height == -1) return -1;
        
        if (std::abs(left_height - right_height) > 1) return -1;
        
        return 1 + std::max(left_height, right_height);
    }
    
    bool is_full_recursive(BSTNode<T>* node) const {
        if (!node) return true;
        if (!node->left && !node->right) return true;
        if (node->left && node->right) {
            return is_full_recursive(node->left.get()) && is_full_recursive(node->right.get());
        }
        return false;
    }
    
    void mirror_recursive(BSTNode<T>* node) {
        if (node) {
            std::swap(node->left, node->right);
            mirror_recursive(node->left.get());
            mirror_recursive(node->right.get());
        }
    }
    
    void copy_tree(BSTNode<T>* node) {
        if (node) {
            insert(node->data);
            copy_tree(node->left.get());
            copy_tree(node->right.get());
        }
    }
    
    void build_balanced_tree(const std::vector<T>& elements, size_t start, size_t end) {
        if (start > end) return;
        
        size_t mid = (start + end) / 2;
        insert(elements[mid]);
        build_balanced_tree(elements, start, mid - 1);
        build_balanced_tree(elements, mid + 1, end);
    }
    
    void print_recursive(BSTNode<T>* node, const std::string& prefix, bool is_left) const {
        if (node) {
            std::cout << prefix;
            std::cout << (is_left ? "├── " : "└── ");
            std::cout << node->data << std::endl;
            
            print_recursive(node->left.get(), prefix + (is_left ? "│   " : "    "), true);
            print_recursive(node->right.get(), prefix + (is_left ? "│   " : "    "), false);
        }
    }
};

// Type aliases for common use cases
using IntBST = BinarySearchTree<int>;
using DoubleBST = BinarySearchTree<double>;
using StringBST = BinarySearchTree<std::string>;

} // namespace structures
} // namespace dsa
