#pragma once
#include <vector>
#include <stdexcept>
#include <iostream>
#include <initializer_list>
#include <functional>
#include "../Utils/Print.hpp"
#include "../Utils/Logger.hpp"

namespace dsa {
namespace structures {

/**
 * @brief Priority Queue Data Structure with template support
 */
template<typename T, typename Compare = std::less<T>>
class PriorityQueue {
private:
    std::vector<T> data;
    Compare comp;
    static constexpr size_t DEFAULT_CAPACITY = 10;

public:
    // Constructors
    PriorityQueue() : data(), comp() {
        data.reserve(DEFAULT_CAPACITY);
    }

    explicit PriorityQueue(const Compare& comparator) : data(), comp(comparator) {
        data.reserve(DEFAULT_CAPACITY);
    }

    PriorityQueue(std::initializer_list<T> init_list, const Compare& comparator = Compare()) 
        : data(init_list.begin(), init_list.end()), comp(comparator) {
        build_heap();
    }

    PriorityQueue(const PriorityQueue& other) : data(other.data), comp(other.comp) {}

    PriorityQueue(PriorityQueue&& other) noexcept 
        : data(std::move(other.data)), comp(std::move(other.comp)) {}

    // Destructor
    ~PriorityQueue() = default;

    // Assignment operators
    PriorityQueue& operator=(const PriorityQueue& other) {
        if (this != &other) {
            data = other.data;
            comp = other.comp;
        }
        return *this;
    }

    PriorityQueue& operator=(PriorityQueue&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);
            comp = std::move(other.comp);
        }
        return *this;
    }

    // Capacity
    bool empty() const { return data.empty(); }
    size_t size() const { return data.size(); }
    size_t max_size() const { return data.max_size(); }

    // Element access
    const T& top() const {
        if (empty()) {
            throw std::out_of_range("PriorityQueue is empty");
        }
        return data[0];
    }

    T& top() {
        if (empty()) {
            throw std::out_of_range("PriorityQueue is empty");
        }
        return data[0];
    }

    // Modifiers
    void push(const T& value) {
        data.push_back(value);
        heapify_up(data.size() - 1);
    }

    void push(T&& value) {
        data.push_back(std::move(value));
        heapify_up(data.size() - 1);
    }

    template<typename... Args>
    void emplace(Args&&... args) {
        data.emplace_back(std::forward<Args>(args)...);
        heapify_up(data.size() - 1);
    }

    void pop() {
        if (empty()) {
            throw std::out_of_range("PriorityQueue is empty");
        }
        data[0] = data.back();
        data.pop_back();
        if (!empty()) {
            heapify_down(0);
        }
    }

    void clear() {
        data.clear();
    }

    // PriorityQueue operations
    void reserve(size_t capacity) {
        data.reserve(capacity);
    }

    void shrink_to_fit() {
        data.shrink_to_fit();
    }

    // Search operations
    bool contains(const T& value) const {
        return std::find(data.begin(), data.end(), value) != data.end();
    }

    size_t count(const T& value) const {
        return std::count(data.begin(), data.end(), value);
    }

    // PriorityQueue manipulation
    void heapify() {
        build_heap();
    }

    // Mathematical operations (for numeric types)
    template<typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, T>::type sum() const {
        T result = T{};
        for (const auto& item : data) {
            result += item;
        }
        return result;
    }

    template<typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, double>::type average() const {
        if (empty()) {
            throw std::out_of_range("PriorityQueue is empty");
        }
        return static_cast<double>(sum()) / size();
    }

    // Iterators
    typename std::vector<T>::iterator begin() { return data.begin(); }
    typename std::vector<T>::const_iterator begin() const { return data.begin(); }
    typename std::vector<T>::iterator end() { return data.end(); }
    typename std::vector<T>::const_iterator end() const { return data.end(); }

    // Reverse iterators
    typename std::vector<T>::reverse_iterator rbegin() { return data.rbegin(); }
    typename std::vector<T>::const_reverse_iterator rbegin() const { return data.rbegin(); }
    typename std::vector<T>::reverse_iterator rend() { return data.rend(); }
    typename std::vector<T>::const_reverse_iterator rend() const { return data.rend(); }

    // Comparison operators
    bool operator==(const PriorityQueue& other) const {
        if (size() != other.size()) return false;
        std::vector<T> this_sorted = data;
        std::vector<T> other_sorted = other.data;
        std::sort(this_sorted.begin(), this_sorted.end());
        std::sort(other_sorted.begin(), other_sorted.end());
        return this_sorted == other_sorted;
    }

    bool operator!=(const PriorityQueue& other) const {
        return !(*this == other);
    }

    // Utility functions
    void print() const {
        std::cout << "PriorityQueue: ";
        for (size_t i = 0; i < data.size(); ++i) {
            std::cout << data[i];
            if (i < data.size() - 1) {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }

    void print_tree() const {
        if (empty()) {
            std::cout << "Empty priority queue" << std::endl;
            return;
        }
        print_tree_recursive(0, "", true);
    }

    std::string to_string() const {
        std::stringstream ss;
        ss << "PriorityQueue[size=" << size() << "]";
        return ss.str();
    }

    // PriorityQueue-specific operations
    bool is_heap() const {
        return std::is_heap(data.begin(), data.end(), comp);
    }

    void merge(PriorityQueue& other) {
        for (const auto& item : other.data) {
            push(item);
        }
        other.clear();
    }

    PriorityQueue extract_top_k(size_t k) {
        PriorityQueue result(comp);
        size_t count = std::min(k, size());
        
        for (size_t i = 0; i < count; ++i) {
            result.push(top());
            pop();
        }
        
        return result;
    }

    std::vector<T> get_top_k(size_t k) {
        std::vector<T> result;
        size_t count = std::min(k, size());
        
        for (size_t i = 0; i < count; ++i) {
            result.push_back(top());
            pop();
        }
        
        // Restore the priority queue
        for (const auto& item : result) {
            push(item);
        }
        
        return result;
    }

    // PriorityQueue validation
    bool is_valid() const {
        return is_heap();
    }

    // Memory management
    size_t capacity() const {
        return data.capacity();
    }

    // Priority queue operations with error handling
    bool try_push(const T& value) {
        try {
            push(value);
            return true;
        } catch (...) {
            return false;
        }
    }

    bool try_pop() {
        try {
            pop();
            return true;
        } catch (...) {
            return false;
        }
    }

    std::pair<bool, T> try_top() const {
        if (empty()) {
            return {false, T{}};
        }
        return {true, data[0]};
    }

    // Change priority operations
    void change_priority(const T& old_value, const T& new_value) {
        auto it = std::find(data.begin(), data.end(), old_value);
        if (it != data.end()) {
            *it = new_value;
            size_t index = std::distance(data.begin(), it);
            
            // Re-heapify from the changed position
            if (comp(data[index], data[(index - 1) / 2])) {
                heapify_up(index);
            } else {
                heapify_down(index);
            }
        }
    }

    void remove(const T& value) {
        auto it = std::find(data.begin(), data.end(), value);
        if (it != data.end()) {
            size_t index = std::distance(data.begin(), it);
            
            // Replace with last element
            data[index] = data.back();
            data.pop_back();
            
            if (index < data.size()) {
                // Re-heapify
                if (index > 0 && comp(data[index], data[(index - 1) / 2])) {
                    heapify_up(index);
                } else {
                    heapify_down(index);
                }
            }
        }
    }

private:
    // Helper functions
    void heapify_up(size_t index) {
        while (index > 0) {
            size_t parent = (index - 1) / 2;
            if (comp(data[parent], data[index])) {
                break;
            }
            std::swap(data[index], data[parent]);
            index = parent;
        }
    }

    void heapify_down(size_t index) {
        size_t heap_size = data.size();
        while (true) {
            size_t largest = index;
            size_t left = 2 * index + 1;
            size_t right = 2 * index + 2;

            if (left < heap_size && comp(data[largest], data[left])) {
                largest = left;
            }

            if (right < heap_size && comp(data[largest], data[right])) {
                largest = right;
            }

            if (largest == index) {
                break;
            }

            std::swap(data[index], data[largest]);
            index = largest;
        }
    }

    void build_heap() {
        for (int i = data.size() / 2 - 1; i >= 0; --i) {
            heapify_down(i);
        }
    }

    void print_tree_recursive(size_t index, const std::string& prefix, bool is_left) const {
        if (index >= data.size()) return;

        std::cout << prefix;
        std::cout << (is_left ? "├── " : "└── ");
        std::cout << data[index] << std::endl;

        size_t left = 2 * index + 1;
        size_t right = 2 * index + 2;

        if (left < data.size()) {
            print_tree_recursive(left, prefix + (is_left ? "│   " : "    "), true);
        }
        if (right < data.size()) {
            print_tree_recursive(right, prefix + (is_left ? "│   " : "    "), false);
        }
    }
};

// Type aliases for common use cases
template<typename T>
using MaxPriorityQueue = PriorityQueue<T, std::less<T>>;

template<typename T>
using MinPriorityQueue = PriorityQueue<T, std::greater<T>>;

using IntMaxPriorityQueue = MaxPriorityQueue<int>;
using IntMinPriorityQueue = MinPriorityQueue<int>;
using DoubleMaxPriorityQueue = MaxPriorityQueue<double>;
using DoubleMinPriorityQueue = MinPriorityQueue<double>;

// Utility functions for priority queue operations
template<typename T, typename Compare>
PriorityQueue<T, Compare> merge_priority_queues(const PriorityQueue<T, Compare>& pq1, 
                                               const PriorityQueue<T, Compare>& pq2) {
    PriorityQueue<T, Compare> result = pq1;
    for (const auto& item : pq2) {
        result.push(item);
    }
    return result;
}

// Custom comparator examples
template<typename T>
struct CustomMaxComparator {
    bool operator()(const T& a, const T& b) const {
        return a < b; // Max heap
    }
};

template<typename T>
struct CustomMinComparator {
    bool operator()(const T& a, const T& b) const {
        return a > b; // Min heap
    }
};

// Priority queue with custom comparators
template<typename T>
using CustomMaxPriorityQueue = PriorityQueue<T, CustomMaxComparator<T>>;

template<typename T>
using CustomMinPriorityQueue = PriorityQueue<T, CustomMinComparator<T>>;

} // namespace structures
} // namespace dsa
