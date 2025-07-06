#pragma once
#include <memory>
#include <stdexcept>
#include <iostream>
#include <initializer_list>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "../Utils/Print.hpp"
#include "../Utils/Logger.hpp"

namespace dsa {
namespace structures {

/**
 * @brief Disjoint Set Node structure
 */
template<typename T>
struct DisjointSetNode {
    T data;
    DisjointSetNode<T>* parent;
    int rank;
    size_t size;
    
    DisjointSetNode(const T& value) : data(value), parent(this), rank(0), size(1) {}
    DisjointSetNode(T&& value) : data(std::move(value)), parent(this), rank(0), size(1) {}
};

/**
 * @brief Disjoint Set Data Structure (Union-Find)
 */
template<typename T>
class DisjointSet {
private:
    std::unordered_map<T, std::unique_ptr<DisjointSetNode<T>>> nodes;
    size_t set_count_;

public:
    // Constructors
    DisjointSet() : set_count_(0) {}
    
    // Constructor with initial size (creates sets for 0 to size-1)
    DisjointSet(size_t size) : set_count_(0) {
        for (size_t i = 0; i < size; ++i) {
            make_set(static_cast<T>(i));
        }
    }
    
    DisjointSet(std::initializer_list<T> init_list) : set_count_(0) {
        for (const auto& item : init_list) {
            make_set(item);
        }
    }
    
    DisjointSet(const DisjointSet& other) : set_count_(0) {
        for (const auto& pair : other.nodes) {
            make_set(pair.first);
        }
    }
    
    DisjointSet(DisjointSet&& other) noexcept 
        : nodes(std::move(other.nodes)), set_count_(other.set_count_) {
        other.set_count_ = 0;
    }
    
    // Destructor
    ~DisjointSet() = default;
    
    // Assignment operators
    DisjointSet& operator=(const DisjointSet& other) {
        if (this != &other) {
            clear();
            for (const auto& pair : other.nodes) {
                make_set(pair.first);
            }
        }
        return *this;
    }
    
    DisjointSet& operator=(DisjointSet&& other) noexcept {
        if (this != &other) {
            clear();
            nodes = std::move(other.nodes);
            set_count_ = other.set_count_;
            other.set_count_ = 0;
        }
        return *this;
    }
    
    // Capacity
    bool empty() const { return nodes.empty(); }
    size_t size() const { return nodes.size(); }
    size_t set_count() const { return set_count_; }
    
    // Modifiers
    void make_set(const T& value) {
        if (nodes.find(value) == nodes.end()) {
            nodes[value] = std::make_unique<DisjointSetNode<T>>(value);
            set_count_++;
        }
    }
    
    void make_set(T&& value) {
        if (nodes.find(value) == nodes.end()) {
            nodes[value] = std::make_unique<DisjointSetNode<T>>(std::move(value));
            set_count_++;
        }
    }
    
    T find_set(const T& value) {
        auto it = nodes.find(value);
        if (it == nodes.end()) {
            throw std::out_of_range("Element not found in disjoint set");
        }
        
        DisjointSetNode<T>* node = it->second.get();
        DisjointSetNode<T>* root = find_set_recursive(node);
        
        // Path compression
        if (root != node) {
            node->parent = root;
        }
        
        return root->data;
    }
    
    const T& find_set(const T& value) const {
        auto it = nodes.find(value);
        if (it == nodes.end()) {
            throw std::out_of_range("Element not found in disjoint set");
        }
        
        DisjointSetNode<T>* node = it->second.get();
        DisjointSetNode<T>* root = find_set_recursive(node);
        return root->data;
    }
    
    void union_sets(const T& x, const T& y) {
        T root_x = find_set(x);
        T root_y = find_set(y);
        
        if (root_x == root_y) {
            return; // Already in the same set
        }
        
        DisjointSetNode<T>* node_x = nodes[root_x].get();
        DisjointSetNode<T>* node_y = nodes[root_y].get();
        
        // Union by rank
        if (node_x->rank < node_y->rank) {
            node_x->parent = node_y;
            node_y->size += node_x->size;
        } else if (node_x->rank > node_y->rank) {
            node_y->parent = node_x;
            node_x->size += node_y->size;
        } else {
            node_y->parent = node_x;
            node_x->rank++;
            node_x->size += node_y->size;
        }
        
        set_count_--;
    }
    
    void clear() {
        nodes.clear();
        set_count_ = 0;
    }
    
    // Set operations
    bool is_same_set(const T& x, const T& y) {
        try {
            return find_set(x) == find_set(y);
        } catch (const std::out_of_range&) {
            return false;
        }
    }
    
    size_t get_set_size(const T& value) {
        T root = find_set(value);
        return nodes[root]->size;
    }
    
    std::vector<T> get_set_elements(const T& value) {
        T root = find_set(value);
        std::vector<T> elements;
        
        for (const auto& pair : nodes) {
            if (find_set(pair.first) == root) {
                elements.push_back(pair.first);
            }
        }
        
        return elements;
    }
    
    std::vector<std::vector<T>> get_all_sets() {
        std::unordered_map<T, std::vector<T>> sets;
        
        for (const auto& pair : nodes) {
            T root = find_set(pair.first);
            sets[root].push_back(pair.first);
        }
        
        std::vector<std::vector<T>> result;
        for (const auto& pair : sets) {
            result.push_back(pair.second);
        }
        
        return result;
    }
    
    // DisjointSet properties
    size_t max_set_size() const {
        size_t max_size = 0;
        for (const auto& pair : nodes) {
            max_size = std::max(max_size, pair.second->size);
        }
        return max_size;
    }
    
    size_t min_set_size() const {
        if (empty()) return 0;
        
        size_t min_size = std::numeric_limits<size_t>::max();
        for (const auto& pair : nodes) {
            min_size = std::min(min_size, pair.second->size);
        }
        return min_size;
    }
    
    double average_set_size() const {
        if (empty()) return 0.0;
        
        size_t total_size = 0;
        for (const auto& pair : nodes) {
            total_size += pair.second->size;
        }
        
        return static_cast<double>(total_size) / set_count_;
    }
    
    // Utility functions
    void print() const {
        std::cout << "DisjointSet:" << std::endl;
        auto sets = const_cast<DisjointSet*>(this)->get_all_sets();
        
        for (size_t i = 0; i < sets.size(); ++i) {
            std::cout << "  Set " << i << ": ";
            utils::Print::printVector(sets[i]);
            std::cout << std::endl;
        }
    }
    
    void print_sets() const {
        std::cout << "DisjointSet sets:" << std::endl;
        for (const auto& pair : nodes) {
            std::cout << "  " << pair.first << " -> " << pair.second->parent->data 
                     << " (rank=" << pair.second->rank << ", size=" << pair.second->size << ")" << std::endl;
        }
    }
    
    std::string to_string() const {
        std::stringstream ss;
        ss << "DisjointSet[elements=" << size() << ", sets=" << set_count_ 
           << ", avg_size=" << average_set_size() << "]";
        return ss.str();
    }
    
    // Iterator support
    class Iterator {
    private:
        typename std::unordered_map<T, std::unique_ptr<DisjointSetNode<T>>>::iterator current;
        typename std::unordered_map<T, std::unique_ptr<DisjointSetNode<T>>>::iterator end;
        
    public:
        Iterator(typename std::unordered_map<T, std::unique_ptr<DisjointSetNode<T>>>::iterator it,
                typename std::unordered_map<T, std::unique_ptr<DisjointSetNode<T>>>::iterator end_it)
            : current(it), end(end_it) {}
        
        T& operator*() { return current->first; }
        const T& operator*() const { return current->first; }
        
        Iterator& operator++() {
            ++current;
            return *this;
        }
        
        Iterator operator++(int) {
            Iterator temp = *this;
            ++current;
            return temp;
        }
        
        bool operator==(const Iterator& other) const {
            return current == other.current;
        }
        
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };
    
    Iterator begin() { return Iterator(nodes.begin(), nodes.end()); }
    Iterator end() { return Iterator(nodes.end(), nodes.end()); }
    Iterator begin() const { return Iterator(nodes.begin(), nodes.end()); }
    Iterator end() const { return Iterator(nodes.end(), nodes.end()); }

private:
    // Helper functions
    DisjointSetNode<T>* find_set_recursive(DisjointSetNode<T>* node) {
        if (node->parent != node) {
            node->parent = find_set_recursive(node->parent);
        }
        return node->parent;
    }
    
    const DisjointSetNode<T>* find_set_recursive(const DisjointSetNode<T>* node) const {
        if (node->parent != node) {
            return find_set_recursive(node->parent);
        }
        return node->parent;
    }
};

// Type aliases for common use cases
using IntDisjointSet = DisjointSet<int>;
using StringDisjointSet = DisjointSet<std::string>;
using CharDisjointSet = DisjointSet<char>;

} // namespace structures
} // namespace dsa
