#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include <cmath>
#include "../Utils/Print.hpp"
#include "../Utils/Logger.hpp"
#include "../Utils/Comparator.hpp"

namespace dsa {
namespace algorithms {

/**
 * @brief Exponential Search Algorithm Implementation
 * Time Complexity: O(log n)
 * Space Complexity: O(1)
 * Requires: Sorted array
 */
template<typename T>
class ExponentialSearch {
public:
    // Standard exponential search
    static size_t search(const std::vector<T>& arr, const T& target) {
        size_t n = arr.size();
        if (n == 0) return static_cast<size_t>(-1);
        
        // If target is the first element
        if (arr[0] == target) return 0;
        
        // Find range for binary search
        size_t i = 1;
        while (i < n && arr[i] <= target) {
            i = i * 2;
        }
        
        // Binary search in the found range
        return binary_search_range(arr, target, i / 2, std::min(i, n - 1));
    }
    
    // Exponential search with custom comparator
    template<typename Compare>
    static size_t search(const std::vector<T>& arr, const T& target, Compare comp) {
        size_t n = arr.size();
        if (n == 0) return static_cast<size_t>(-1);
        
        // If target is the first element
        if (!comp(arr[0], target) && !comp(target, arr[0])) return 0;
        
        // Find range for binary search
        size_t i = 1;
        while (i < n && (comp(arr[i], target) || !comp(target, arr[i]))) {
            i = i * 2;
        }
        
        // Binary search in the found range
        return binary_search_range(arr, target, i / 2, std::min(i, n - 1), comp);
    }
    
    // Exponential search with custom step size
    static size_t search_with_step(const std::vector<T>& arr, const T& target, size_t step_multiplier) {
        size_t n = arr.size();
        if (n == 0) return static_cast<size_t>(-1);
        
        // If target is the first element
        if (arr[0] == target) return 0;
        
        // Find range for binary search
        size_t i = 1;
        while (i < n && arr[i] <= target) {
            i = i * step_multiplier;
        }
        
        // Binary search in the found range
        return binary_search_range(arr, target, i / step_multiplier, std::min(i, n - 1));
    }
    
    // Exponential search with early termination
    static size_t search_early_termination(const std::vector<T>& arr, const T& target) {
        size_t n = arr.size();
        if (n == 0) return static_cast<size_t>(-1);
        
        // If target is the first element
        if (arr[0] == target) return 0;
        
        // Find range for binary search with early termination
        size_t i = 1;
        while (i < n) {
            if (arr[i] == target) return i;
            if (arr[i] > target) break;
            i = i * 2;
        }
        
        // Binary search in the found range
        return binary_search_range(arr, target, i / 2, std::min(i, n - 1));
    }
    
    // Exponential search for unbounded arrays (simulated)
    static size_t search_unbounded(const std::vector<T>& arr, const T& target) {
        size_t n = arr.size();
        if (n == 0) return static_cast<size_t>(-1);
        
        // If target is the first element
        if (arr[0] == target) return 0;
        
        // Find range for binary search (simulating unbounded array)
        size_t i = 1;
        while (i < n && arr[i] <= target) {
            i = i * 2;
        }
        
        // Binary search in the found range
        return binary_search_range(arr, target, i / 2, std::min(i, n - 1));
    }
    
    // Exponential search with range bounds
    static size_t search_with_bounds(const std::vector<T>& arr, const T& target, 
                                   size_t left, size_t right) {
        if (left >= right) return static_cast<size_t>(-1);
        
        // If target is the first element
        if (arr[left] == target) return left;
        
        // Find range for binary search
        size_t i = left + 1;
        while (i < right && arr[i] <= target) {
            i = left + (i - left) * 2;
        }
        
        // Binary search in the found range
        return binary_search_range(arr, target, (i - left) / 2 + left, std::min(i, right));
    }
    
    // Exponential search for finding insertion point
    static size_t find_insertion_point(const std::vector<T>& arr, const T& target) {
        size_t n = arr.size();
        if (n == 0) return 0;
        
        // If target is less than first element
        if (target < arr[0]) return 0;
        
        // Find range for binary search
        size_t i = 1;
        while (i < n && arr[i] < target) {
            i = i * 2;
        }
        
        // Binary search for insertion point
        return binary_search_insertion_point(arr, target, i / 2, std::min(i, n - 1));
    }
    
    // Exponential search with custom key function
    template<typename KeyFunc>
    static size_t search_with_key(const std::vector<T>& arr, const T& target, KeyFunc key_func) {
        size_t n = arr.size();
        if (n == 0) return static_cast<size_t>(-1);
        
        // If target is the first element
        if (key_func(arr[0]) == key_func(target)) return 0;
        
        // Find range for binary search
        size_t i = 1;
        while (i < n && key_func(arr[i]) <= key_func(target)) {
            i = i * 2;
        }
        
        // Binary search in the found range
        return binary_search_range_with_key(arr, target, i / 2, std::min(i, n - 1), key_func);
    }
    
    // Utility functions
    static bool contains(const std::vector<T>& arr, const T& target) {
        return search(arr, target) != static_cast<size_t>(-1);
    }
    
    static size_t find_first_occurrence(const std::vector<T>& arr, const T& target) {
        size_t pos = search(arr, target);
        if (pos == static_cast<size_t>(-1)) return static_cast<size_t>(-1);
        
        // Find first occurrence
        while (pos > 0 && arr[pos - 1] == target) {
            --pos;
        }
        
        return pos;
    }
    
    static size_t find_last_occurrence(const std::vector<T>& arr, const T& target) {
        size_t pos = search(arr, target);
        if (pos == static_cast<size_t>(-1)) return static_cast<size_t>(-1);
        
        // Find last occurrence
        while (pos + 1 < arr.size() && arr[pos + 1] == target) {
            ++pos;
        }
        
        return pos;
    }
    
    static std::pair<size_t, size_t> find_range(const std::vector<T>& arr, const T& target) {
        size_t first = find_first_occurrence(arr, target);
        if (first == static_cast<size_t>(-1)) {
            return {static_cast<size_t>(-1), static_cast<size_t>(-1)};
        }
        
        size_t last = find_last_occurrence(arr, target);
        return {first, last};
    }
    
    // Get optimal step size for given array size
    static size_t get_optimal_step_size(size_t array_size) {
        if (array_size <= 1) return 1;
        return static_cast<size_t>(std::log2(array_size));
    }
    
    // Calculate search complexity
    static double get_complexity(size_t array_size) {
        if (array_size == 0) return 0.0;
        return std::log2(array_size);
    }

private:
    // Binary search in range
    static size_t binary_search_range(const std::vector<T>& arr, const T& target, 
                                    size_t left, size_t right) {
        while (left <= right) {
            size_t mid = left + (right - left) / 2;
            
            if (arr[mid] == target) {
                return mid;
            } else if (arr[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        return static_cast<size_t>(-1);
    }
    
    template<typename Compare>
    static size_t binary_search_range(const std::vector<T>& arr, const T& target, 
                                    size_t left, size_t right, Compare comp) {
        while (left <= right) {
            size_t mid = left + (right - left) / 2;
            
            if (!comp(arr[mid], target) && !comp(target, arr[mid])) {
                return mid;
            } else if (comp(arr[mid], target)) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        return static_cast<size_t>(-1);
    }
    
    // Binary search for insertion point
    static size_t binary_search_insertion_point(const std::vector<T>& arr, const T& target, 
                                              size_t left, size_t right) {
        while (left <= right) {
            size_t mid = left + (right - left) / 2;
            
            if (arr[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        return left;
    }
    
    // Binary search with custom key function
    template<typename KeyFunc>
    static size_t binary_search_range_with_key(const std::vector<T>& arr, const T& target, 
                                             size_t left, size_t right, KeyFunc key_func) {
        while (left <= right) {
            size_t mid = left + (right - left) / 2;
            
            if (key_func(arr[mid]) == key_func(target)) {
                return mid;
            } else if (key_func(arr[mid]) < key_func(target)) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        return static_cast<size_t>(-1);
    }
};

// Type aliases for common use cases
using IntExponentialSearch = ExponentialSearch<int>;
using DoubleExponentialSearch = ExponentialSearch<double>;
using StringExponentialSearch = ExponentialSearch<std::string>;

} // namespace algorithms
} // namespace dsa
