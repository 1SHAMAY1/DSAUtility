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
 * @brief Heap Data Structure with template support
 */
template<typename T, typename Compare = std::less<T>>
class Heap {
private:
    std::vector<T> data;
    Compare comp;
    static constexpr size_t DEFAULT_CAPACITY = 10;

public:
    // Constructors
    Heap() : data(), comp() {
        data.reserve(DEFAULT_CAPACITY);
    }

    explicit Heap(const Compare& comparator) : data(), comp(comparator) {
        data.reserve(DEFAULT_CAPACITY);
    }

    Heap(std::initializer_list<T> init_list, const Compare& comparator = Compare()) 
        : data(init_list.begin(), init_list.end()), comp(comparator) {
        build_heap();
    }

    Heap(const Heap& other) : data(other.data), comp(other.comp) {}

    Heap(Heap&& other) noexcept 
        : data(std::move(other.data)), comp(std::move(other.comp)) {}

    // Destructor
    ~Heap() = default;

    // Assignment operators
    Heap& operator=(const Heap& other) {
        if (this != &other) {
            data = other.data;
            comp = other.comp;
        }
        return *this;
    }

    Heap& operator=(Heap&& other) noexcept {
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
            throw std::out_of_range("Heap is empty");
        }
        return data[0];
    }

    T& top() {
        if (empty()) {
            throw std::out_of_range("Heap is empty");
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
            throw std::out_of_range("Heap is empty");
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

    // Heap operations
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

    // Heap manipulation
    void heapify() {
        build_heap();
    }

    void sort() {
        std::vector<T> temp = data;
        size_t heap_size = temp.size();
        
        // Build heap
        for (int i = heap_size / 2 - 1; i >= 0; --i) {
            heapify_down_internal(temp, i, heap_size);
        }
        
        // Extract elements one by one
        for (int i = heap_size - 1; i > 0; --i) {
            std::swap(temp[0], temp[i]);
            heapify_down_internal(temp, 0, i);
        }
        
        data = temp;
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
            throw std::out_of_range("Heap is empty");
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
    bool operator==(const Heap& other) const {
        if (size() != other.size()) return false;
        std::vector<T> this_sorted = data;
        std::vector<T> other_sorted = other.data;
        std::sort(this_sorted.begin(), this_sorted.end());
        std::sort(other_sorted.begin(), other_sorted.end());
        return this_sorted == other_sorted;
    }

    bool operator!=(const Heap& other) const {
        return !(*this == other);
    }

    // Utility functions
    void print() const {
        std::cout << "Heap: ";
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
            std::cout << "Empty heap" << std::endl;
            return;
        }
        print_tree_recursive(0, "", true);
    }

    std::string to_string() const {
        std::stringstream ss;
        ss << "Heap[size=" << size() << "]";
        return ss.str();
    }

    // Heap-specific operations
    bool is_heap() const {
        return std::is_heap(data.begin(), data.end(), comp);
    }

    void merge(Heap& other) {
        for (const auto& item : other.data) {
            push(item);
        }
        other.clear();
    }

    Heap extract_top_k(size_t k) {
        Heap result(comp);
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
        
        // Restore the heap
        for (const auto& item : result) {
            push(item);
        }
        
        return result;
    }

    // Heap validation
    bool is_valid() const {
        return is_heap();
    }

    // Memory management
    size_t capacity() const {
        return data.capacity();
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

    void heapify_down_internal(std::vector<T>& arr, size_t index, size_t heap_size) {
        while (true) {
            size_t largest = index;
            size_t left = 2 * index + 1;
            size_t right = 2 * index + 2;

            if (left < heap_size && comp(arr[largest], arr[left])) {
                largest = left;
            }

            if (right < heap_size && comp(arr[largest], arr[right])) {
                largest = right;
            }

            if (largest == index) {
                break;
            }

            std::swap(arr[index], arr[largest]);
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
using MaxHeap = Heap<T, std::less<T>>;

template<typename T>
using MinHeap = Heap<T, std::greater<T>>;

using IntMaxHeap = MaxHeap<int>;
using IntMinHeap = MinHeap<int>;
using DoubleMaxHeap = MaxHeap<double>;
using DoubleMinHeap = MinHeap<double>;

// Utility functions for heap operations
template<typename T, typename Compare>
void heap_sort(std::vector<T>& arr, Compare comp = Compare()) {
    Heap<T, Compare> heap(arr, comp);
    heap.sort();
    arr = std::vector<T>(heap.begin(), heap.end());
}

template<typename T>
void heap_sort(std::vector<T>& arr) {
    heap_sort<T, std::less<T>>(arr);
}

template<typename T, typename Compare>
Heap<T, Compare> merge_heaps(const Heap<T, Compare>& heap1, const Heap<T, Compare>& heap2) {
    Heap<T, Compare> result = heap1;
    for (const auto& item : heap2) {
        result.push(item);
    }
    return result;
}

} // namespace structures
} // namespace dsa
