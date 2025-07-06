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
 * @brief Base Node structure for Tree
 */
template<typename T>
struct TreeNode {
    T data;
    std::unique_ptr<TreeNode<T>> left;
    std::unique_ptr<TreeNode<T>> right;
    TreeNode<T>* parent;
    
    TreeNode(const T& value) : data(value), left(nullptr), right(nullptr), parent(nullptr) {}
    TreeNode(T&& value) : data(std::move(value)), left(nullptr), right(nullptr), parent(nullptr) {}
};

/**
 * @brief Base Tree Data Structure
 */
template<typename T>
class Tree {
protected:
    std::unique_ptr<TreeNode<T>> root;
    size_t size_;

public:
    // Constructors
    Tree() : root(nullptr), size_(0) {}
    
    Tree(std::initializer_list<T> init_list) : root(nullptr), size_(0) {
        for (const auto& item : init_list) {
            insert(item);
        }
    }
    
    Tree(const Tree& other) : root(nullptr), size_(0) {
        copy_tree(other.root.get());
    }
    
    Tree(Tree&& other) noexcept 
        : root(std::move(other.root)), size_(other.size_) {
        other.size_ = 0;
    }
    
    // Destructor
    virtual ~Tree() = default;
    
    // Assignment operators
    Tree& operator=(const Tree& other) {
        if (this != &other) {
            clear();
            copy_tree(other.root.get());
        }
        return *this;
    }
    
    Tree& operator=(Tree&& other) noexcept {
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
    virtual T& min() {
        if (empty()) {
            throw std::out_of_range("Tree is empty");
        }
        return find_min(root.get())->data;
    }
    
    virtual const T& min() const {
        if (empty()) {
            throw std::out_of_range("Tree is empty");
        }
        return find_min(root.get())->data;
    }
    
    virtual T& max() {
        if (empty()) {
            throw std::out_of_range("Tree is empty");
        }
        return find_max(root.get())->data;
    }
    
    virtual const T& max() const {
        if (empty()) {
            throw std::out_of_range("Tree is empty");
        }
        return find_max(root.get())->data;
    }
    
    // Modifiers
    virtual void clear() {
        root.reset();
        size_ = 0;
    }
    
    virtual void insert(const T& value) = 0;
    virtual void insert(T&& value) = 0;
    virtual void remove(const T& value) = 0;
    
    // Search operations
    virtual bool contains(const T& value) const {
        return find_node(root.get(), value) != nullptr;
    }
    
    virtual T* find(const T& value) {
        TreeNode<T>* node = find_node(root.get(), value);
        return node ? &node->data : nullptr;
    }
    
    virtual const T* find(const T& value) const {
        TreeNode<T>* node = find_node(root.get(), value);
        return node ? &node->data : nullptr;
    }
    
    // Tree traversal
    virtual std::vector<T> inorder() const {
        std::vector<T> result;
        inorder_recursive(root.get(), result);
        return result;
    }
    
    virtual std::vector<T> preorder() const {
        std::vector<T> result;
        preorder_recursive(root.get(), result);
        return result;
    }
    
    virtual std::vector<T> postorder() const {
        std::vector<T> result;
        postorder_recursive(root.get(), result);
        return result;
    }
    
    virtual std::vector<T> levelorder() const {
        std::vector<T> result;
        if (!root) return result;
        
        std::queue<TreeNode<T>*> q;
        q.push(root.get());
        
        while (!q.empty()) {
            TreeNode<T>* current = q.front();
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
    virtual size_t height() const {
        return height_recursive(root.get());
    }
    
    virtual size_t depth(const T& value) const {
        TreeNode<T>* node = find_node(root.get(), value);
        if (!node) return static_cast<size_t>(-1);
        
        size_t depth = 0;
        TreeNode<T>* current = root.get();
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
    
    virtual bool is_balanced() const {
        return is_balanced_recursive(root.get()) != -1;
    }
    
    virtual bool is_complete() const {
        if (!root) return true;
        
        std::queue<TreeNode<T>*> q;
        q.push(root.get());
        bool found_null = false;
        
        while (!q.empty()) {
            TreeNode<T>* current = q.front();
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
    
    virtual bool is_full() const {
        return is_full_recursive(root.get());
    }
    
    virtual bool is_perfect() const {
        size_t h = height();
        return size() == (1ULL << h) - 1;
    }
    
    // Tree operations
    virtual void balance() {
        std::vector<T> elements = inorder();
        clear();
        build_balanced_tree(elements, 0, elements.size() - 1);
    }
    
    virtual void mirror() {
        mirror_recursive(root.get());
    }
    
    virtual std::unique_ptr<Tree<T>> get_left_subtree() const {
        auto subtree = std::make_unique<Tree<T>>();
        if (root && root->left) {
            subtree->copy_tree(root->left.get());
        }
        return subtree;
    }
    
    virtual std::unique_ptr<Tree<T>> get_right_subtree() const {
        auto subtree = std::make_unique<Tree<T>>();
        if (root && root->right) {
            subtree->copy_tree(root->right.get());
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
    virtual bool operator==(const Tree& other) const {
        if (size_ != other.size_) return false;
        std::vector<T> this_inorder = inorder();
        std::vector<T> other_inorder = other.inorder();
        return this_inorder == other_inorder;
    }
    
    virtual bool operator!=(const Tree& other) const {
        return !(*this == other);
    }
    
    // Utility functions
    virtual void print() const {
        print_recursive(root.get(), "", true);
    }
    
    virtual void print_inorder() const {
        std::vector<T> elements = inorder();
        utils::Print::printVectorWithPrefix(elements, "Inorder: ");
        std::cout << std::endl;
    }
    
    virtual void print_preorder() const {
        std::vector<T> elements = preorder();
        utils::Print::printVectorWithPrefix(elements, "Preorder: ");
        std::cout << std::endl;
    }
    
    virtual void print_postorder() const {
        std::vector<T> elements = postorder();
        utils::Print::printVectorWithPrefix(elements, "Postorder: ");
        std::cout << std::endl;
    }
    
    virtual void print_levelorder() const {
        std::vector<T> elements = levelorder();
        utils::Print::printVectorWithPrefix(elements, "Levelorder: ");
        std::cout << std::endl;
    }
    
    virtual std::string to_string() const {
        std::stringstream ss;
        ss << "Tree[size=" << size_ << ", height=" << height() << "]";
        return ss.str();
    }
    
    // Iterator support (basic implementation)
    class Iterator {
    private:
        std::vector<T> elements;
        size_t current_index;
        
    public:
        Iterator(const Tree<T>& tree, bool end = false) 
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

protected:
    // Helper functions
    virtual TreeNode<T>* find_min(TreeNode<T>* node) const {
        if (!node || !node->left) return node;
        return find_min(node->left.get());
    }
    
    virtual TreeNode<T>* find_max(TreeNode<T>* node) const {
        if (!node || !node->right) return node;
        return find_max(node->right.get());
    }
    
    virtual TreeNode<T>* find_node(TreeNode<T>* node, const T& value) const {
        if (!node || node->data == value) return node;
        if (value < node->data) {
            return find_node(node->left.get(), value);
        } else {
            return find_node(node->right.get(), value);
        }
    }
    
    virtual void inorder_recursive(TreeNode<T>* node, std::vector<T>& result) const {
        if (node) {
            inorder_recursive(node->left.get(), result);
            result.push_back(node->data);
            inorder_recursive(node->right.get(), result);
        }
    }
    
    virtual void preorder_recursive(TreeNode<T>* node, std::vector<T>& result) const {
        if (node) {
            result.push_back(node->data);
            preorder_recursive(node->left.get(), result);
            preorder_recursive(node->right.get(), result);
        }
    }
    
    virtual void postorder_recursive(TreeNode<T>* node, std::vector<T>& result) const {
        if (node) {
            postorder_recursive(node->left.get(), result);
            postorder_recursive(node->right.get(), result);
            result.push_back(node->data);
        }
    }
    
    virtual size_t height_recursive(TreeNode<T>* node) const {
        if (!node) return 0;
        return 1 + std::max(height_recursive(node->left.get()), height_recursive(node->right.get()));
    }
    
    virtual int is_balanced_recursive(TreeNode<T>* node) const {
        if (!node) return 0;
        
        int left_height = is_balanced_recursive(node->left.get());
        if (left_height == -1) return -1;
        
        int right_height = is_balanced_recursive(node->right.get());
        if (right_height == -1) return -1;
        
        if (std::abs(left_height - right_height) > 1) return -1;
        
        return 1 + std::max(left_height, right_height);
    }
    
    virtual bool is_full_recursive(TreeNode<T>* node) const {
        if (!node) return true;
        if (!node->left && !node->right) return true;
        if (node->left && node->right) {
            return is_full_recursive(node->left.get()) && is_full_recursive(node->right.get());
        }
        return false;
    }
    
    virtual void mirror_recursive(TreeNode<T>* node) {
        if (node) {
            std::swap(node->left, node->right);
            mirror_recursive(node->left.get());
            mirror_recursive(node->right.get());
        }
    }
    
    virtual void copy_tree(TreeNode<T>* node) {
        if (node) {
            insert(node->data);
            copy_tree(node->left.get());
            copy_tree(node->right.get());
        }
    }
    
    virtual void build_balanced_tree(const std::vector<T>& elements, size_t start, size_t end) {
        if (start > end) return;
        
        size_t mid = (start + end) / 2;
        insert(elements[mid]);
        build_balanced_tree(elements, start, mid - 1);
        build_balanced_tree(elements, mid + 1, end);
    }
    
    virtual void print_recursive(TreeNode<T>* node, const std::string& prefix, bool is_left) const {
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
using IntTree = Tree<int>;
using DoubleTree = Tree<double>;
using StringTree = Tree<std::string>;

} // namespace structures
} // namespace dsa
