#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include "../Utils/Print.hpp"
#include "../Utils/Logger.hpp"
#include "../Utils/Comparator.hpp"
#include "../Utils/Swap.hpp"

namespace dsa {
namespace algorithms {

/**
 * @brief HeapSort Algorithm Implementation
 * Time Complexity: O(n log n)
 * Space Complexity: O(1) - in-place sorting
 */
template<typename T>
class HeapSort {
public:
    // Standard heap sort
    static void sort(std::vector<T>& arr) {
        size_t n = arr.size();
        if (n <= 1) return;
        
        // Build max heap
        for (int i = n / 2 - 1; i >= 0; --i) {
            heapify(arr, n, i);
        }
        
        // Extract elements from heap one by one
        for (int i = n - 1; i > 0; --i) {
            utils::Swap::swap(arr[0], arr[i]);
            heapify(arr, i, 0);
        }
    }
    
    // Heap sort with custom comparator
    template<typename Compare>
    static void sort(std::vector<T>& arr, Compare comp) {
        size_t n = arr.size();
        if (n <= 1) return;
        
        // Build heap
        for (int i = n / 2 - 1; i >= 0; --i) {
            heapify(arr, n, i, comp);
        }
        
        // Extract elements from heap one by one
        for (int i = n - 1; i > 0; --i) {
            utils::Swap::swap(arr[0], arr[i]);
            heapify(arr, i, 0, comp);
        }
    }
    
    // Min heap sort (ascending order)
    static void min_heap_sort(std::vector<T>& arr) {
        size_t n = arr.size();
        if (n <= 1) return;
        
        // Build min heap
        for (int i = n / 2 - 1; i >= 0; --i) {
            min_heapify(arr, n, i);
        }
        
        // Extract elements from heap one by one
        for (int i = n - 1; i > 0; --i) {
            utils::Swap::swap(arr[0], arr[i]);
            min_heapify(arr, i, 0);
        }
    }
    
    // Bottom-up heap sort (more efficient)
    static void bottom_up_sort(std::vector<T>& arr) {
        size_t n = arr.size();
        if (n <= 1) return;
        
        // Build heap using bottom-up approach
        for (int i = n / 2 - 1; i >= 0; --i) {
            bottom_up_heapify(arr, n, i);
        }
        
        // Extract elements from heap one by one
        for (int i = n - 1; i > 0; --i) {
            utils::Swap::swap(arr[0], arr[i]);
            bottom_up_heapify(arr, i, 0);
        }
    }
    
    // Heap sort with heap size tracking
    static void sort_with_tracking(std::vector<T>& arr) {
        size_t n = arr.size();
        if (n <= 1) return;
        
        size_t heap_size = n;
        
        // Build max heap
        for (int i = n / 2 - 1; i >= 0; --i) {
            heapify_with_size(arr, heap_size, i);
        }
        
        // Extract elements from heap one by one
        for (int i = n - 1; i > 0; --i) {
            utils::Swap::swap(arr[0], arr[i]);
            --heap_size;
            heapify_with_size(arr, heap_size, 0);
        }
    }
    
    // Utility functions
    static bool is_heap(const std::vector<T>& arr) {
        return is_heap_recursive(arr, 0, arr.size());
    }
    
    static bool is_heap_until(const std::vector<T>& arr, size_t& pos) {
        pos = 0;
        size_t n = arr.size();
        
        for (size_t i = 0; i < n; ++i) {
            size_t left = 2 * i + 1;
            size_t right = 2 * i + 2;
            
            if (left < n && arr[i] < arr[left]) {
                pos = i;
                return false;
            }
            
            if (right < n && arr[i] < arr[right]) {
                pos = i;
                return false;
            }
        }
        
        return true;
    }
    
    static void make_heap(std::vector<T>& arr) {
        size_t n = arr.size();
        for (int i = n / 2 - 1; i >= 0; --i) {
            heapify(arr, n, i);
        }
    }
    
    static void push_heap(std::vector<T>& arr) {
        size_t n = arr.size();
        if (n <= 1) return;
        
        size_t child = n - 1;
        size_t parent = (child - 1) / 2;
        
        while (child > 0 && arr[parent] < arr[child]) {
            utils::Swap::swap(arr[parent], arr[child]);
            child = parent;
            parent = (child - 1) / 2;
        }
    }
    
    static void pop_heap(std::vector<T>& arr) {
        size_t n = arr.size();
        if (n <= 1) return;
        
        utils::Swap::swap(arr[0], arr[n - 1]);
        heapify(arr, n - 1, 0);
    }
    
    static T heap_max(const std::vector<T>& arr) {
        if (arr.empty()) {
            throw std::runtime_error("Heap is empty");
        }
        return arr[0];
    }
    
    static T heap_extract_max(std::vector<T>& arr) {
        if (arr.empty()) {
            throw std::runtime_error("Heap is empty");
        }
        
        T max_val = arr[0];
        arr[0] = arr.back();
        arr.pop_back();
        
        if (!arr.empty()) {
            heapify(arr, arr.size(), 0);
        }
        
        return max_val;
    }
    
    static void heap_increase_key(std::vector<T>& arr, size_t index, T new_key) {
        if (index >= arr.size()) {
            throw std::out_of_range("Index out of range");
        }
        
        if (new_key < arr[index]) {
            throw std::invalid_argument("New key is smaller than current key");
        }
        
        arr[index] = new_key;
        
        while (index > 0 && arr[(index - 1) / 2] < arr[index]) {
            utils::Swap::swap(arr[index], arr[(index - 1) / 2]);
            index = (index - 1) / 2;
        }
    }
    
    static void heap_insert(std::vector<T>& arr, T key) {
        arr.push_back(key);
        push_heap(arr);
    }
    
    // Heap sort with custom heap property
    template<typename HeapProperty>
    static void sort_with_property(std::vector<T>& arr, HeapProperty prop) {
        size_t n = arr.size();
        if (n <= 1) return;
        
        // Build heap
        for (int i = n / 2 - 1; i >= 0; --i) {
            heapify_with_property(arr, n, i, prop);
        }
        
        // Extract elements from heap one by one
        for (int i = n - 1; i > 0; --i) {
            utils::Swap::swap(arr[0], arr[i]);
            heapify_with_property(arr, i, 0, prop);
        }
    }
    
    // Get heap height
    static size_t heap_height(const std::vector<T>& arr) {
        size_t n = arr.size();
        if (n == 0) return 0;
        
        size_t height = 0;
        size_t nodes = 1;
        
        while (nodes <= n) {
            nodes = 2 * nodes + 1;
            ++height;
        }
        
        return height - 1;
    }
    
    // Get number of leaves in heap
    static size_t heap_leaves(const std::vector<T>& arr) {
        size_t n = arr.size();
        if (n == 0) return 0;
        
        return (n + 1) / 2;
    }
    
    // Get heap level of an element
    static size_t heap_level(size_t index) {
        if (index == 0) return 0;
        
        size_t level = 0;
        while (index > 0) {
            index = (index - 1) / 2;
            ++level;
        }
        
        return level;
    }

private:
    // Standard heapify function (max heap)
    static void heapify(std::vector<T>& arr, size_t n, size_t i) {
        size_t largest = i;
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;
        
        if (left < n && arr[left] > arr[largest]) {
            largest = left;
        }
        
        if (right < n && arr[right] > arr[largest]) {
            largest = right;
        }
        
        if (largest != i) {
            utils::Swap::swap(arr[i], arr[largest]);
            heapify(arr, n, largest);
        }
    }
    
    template<typename Compare>
    static void heapify(std::vector<T>& arr, size_t n, size_t i, Compare comp) {
        size_t largest = i;
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;
        
        if (left < n && comp(arr[largest], arr[left])) {
            largest = left;
        }
        
        if (right < n && comp(arr[largest], arr[right])) {
            largest = right;
        }
        
        if (largest != i) {
            utils::Swap::swap(arr[i], arr[largest]);
            heapify(arr, n, largest, comp);
        }
    }
    
    // Min heapify function
    static void min_heapify(std::vector<T>& arr, size_t n, size_t i) {
        size_t smallest = i;
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;
        
        if (left < n && arr[left] < arr[smallest]) {
            smallest = left;
        }
        
        if (right < n && arr[right] < arr[smallest]) {
            smallest = right;
        }
        
        if (smallest != i) {
            utils::Swap::swap(arr[i], arr[smallest]);
            min_heapify(arr, n, smallest);
        }
    }
    
    // Bottom-up heapify (more efficient)
    static void bottom_up_heapify(std::vector<T>& arr, size_t n, size_t i) {
        size_t largest = i;
        
        while (true) {
            size_t left = 2 * i + 1;
            size_t right = 2 * i + 2;
            
            if (left < n && arr[left] > arr[largest]) {
                largest = left;
            }
            
            if (right < n && arr[right] > arr[largest]) {
                largest = right;
            }
            
            if (largest == i) {
                break;
            }
            
            utils::Swap::swap(arr[i], arr[largest]);
            i = largest;
        }
    }
    
    // Heapify with heap size tracking
    static void heapify_with_size(std::vector<T>& arr, size_t heap_size, size_t i) {
        size_t largest = i;
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;
        
        if (left < heap_size && arr[left] > arr[largest]) {
            largest = left;
        }
        
        if (right < heap_size && arr[right] > arr[largest]) {
            largest = right;
        }
        
        if (largest != i) {
            utils::Swap::swap(arr[i], arr[largest]);
            heapify_with_size(arr, heap_size, largest);
        }
    }
    
    // Heapify with custom heap property
    template<typename HeapProperty>
    static void heapify_with_property(std::vector<T>& arr, size_t n, size_t i, HeapProperty prop) {
        size_t largest = i;
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;
        
        if (left < n && prop(arr[left], arr[largest])) {
            largest = left;
        }
        
        if (right < n && prop(arr[right], arr[largest])) {
            largest = right;
        }
        
        if (largest != i) {
            utils::Swap::swap(arr[i], arr[largest]);
            heapify_with_property(arr, n, largest, prop);
        }
    }
    
    // Check if array is a heap recursively
    static bool is_heap_recursive(const std::vector<T>& arr, size_t i, size_t n) {
        if (i >= n) return true;
        
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;
        
        // Check if current node is greater than or equal to its children
        if (left < n && arr[i] < arr[left]) return false;
        if (right < n && arr[i] < arr[right]) return false;
        
        // Recursively check left and right subtrees
        return is_heap_recursive(arr, left, n) && is_heap_recursive(arr, right, n);
    }
};

// Type aliases for common use cases
using IntHeapSort = HeapSort<int>;
using DoubleHeapSort = HeapSort<double>;
using StringHeapSort = HeapSort<std::string>;

} // namespace algorithms
} // namespace dsa
