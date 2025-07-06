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
 * @brief Interpolation Search Algorithm Implementation
 * Time Complexity: O(log log n) average, O(n) worst case
 * Space Complexity: O(1)
 * Requires: Sorted array with uniform distribution
 */
template<typename T>
class InterpolationSearch {
public:
    // Standard interpolation search
    static size_t search(const std::vector<T>& arr, const T& target) {
        size_t left = 0;
        size_t right = arr.size() - 1;
        
        while (left <= right && target >= arr[left] && target <= arr[right]) {
            if (left == right) {
                if (arr[left] == target) return left;
                return static_cast<size_t>(-1);
            }
            
            // Interpolation formula
            size_t pos = left + static_cast<size_t>((target - arr[left]) * (right - left) / (arr[right] - arr[left]));
            
            if (arr[pos] == target) {
                return pos;
            } else if (arr[pos] < target) {
                left = pos + 1;
            } else {
                right = pos - 1;
            }
        }
        
        return static_cast<size_t>(-1);
    }
    
    // Interpolation search with custom comparator
    template<typename Compare>
    static size_t search(const std::vector<T>& arr, const T& target, Compare comp) {
        size_t left = 0;
        size_t right = arr.size() - 1;
        
        while (left <= right && !comp(target, arr[left]) && !comp(arr[right], target)) {
            if (left == right) {
                if (!comp(arr[left], target) && !comp(target, arr[left])) return left;
                return static_cast<size_t>(-1);
            }
            
            // Interpolation formula with custom comparator
            size_t pos = left + static_cast<size_t>((target - arr[left]) * (right - left) / (arr[right] - arr[left]));
            
            if (!comp(arr[pos], target) && !comp(target, arr[pos])) {
                return pos;
            } else if (comp(arr[pos], target)) {
                left = pos + 1;
            } else {
                right = pos - 1;
            }
        }
        
        return static_cast<size_t>(-1);
    }
    
    // Interpolation search with bounds checking
    static size_t search_with_bounds(const std::vector<T>& arr, const T& target, 
                                   size_t left, size_t right) {
        while (left <= right && target >= arr[left] && target <= arr[right]) {
            if (left == right) {
                if (arr[left] == target) return left;
                return static_cast<size_t>(-1);
            }
            
            // Interpolation formula
            size_t pos = left + static_cast<size_t>((target - arr[left]) * (right - left) / (arr[right] - arr[left]));
            
            // Bounds checking
            if (pos < left || pos > right) {
                pos = left + (right - left) / 2; // Fallback to binary search
            }
            
            if (arr[pos] == target) {
                return pos;
            } else if (arr[pos] < target) {
                left = pos + 1;
            } else {
                right = pos - 1;
            }
        }
        
        return static_cast<size_t>(-1);
    }
    
    // Interpolation search with fallback to binary search
    static size_t search_with_fallback(const std::vector<T>& arr, const T& target) {
        size_t left = 0;
        size_t right = arr.size() - 1;
        size_t iterations = 0;
        const size_t max_iterations = static_cast<size_t>(std::log2(arr.size())) + 1;
        
        while (left <= right && target >= arr[left] && target <= arr[right]) {
            if (left == right) {
                if (arr[left] == target) return left;
                return static_cast<size_t>(-1);
            }
            
            // Interpolation formula
            size_t pos = left + static_cast<size_t>((target - arr[left]) * (right - left) / (arr[right] - arr[left]));
            
            if (arr[pos] == target) {
                return pos;
            } else if (arr[pos] < target) {
                left = pos + 1;
            } else {
                right = pos - 1;
            }
            
            // Fallback to binary search if too many iterations
            if (++iterations > max_iterations) {
                return binary_search_fallback(arr, target, left, right);
            }
        }
        
        return static_cast<size_t>(-1);
    }
    
    // Interpolation search for floating point numbers
    static size_t search_floats(const std::vector<double>& arr, double target) {
        size_t left = 0;
        size_t right = arr.size() - 1;
        const double epsilon = 1e-10;
        
        while (left <= right && target >= arr[left] - epsilon && target <= arr[right] + epsilon) {
            if (left == right) {
                if (std::abs(arr[left] - target) < epsilon) return left;
                return static_cast<size_t>(-1);
            }
            
            // Interpolation formula for floats
            size_t pos = left + static_cast<size_t>((target - arr[left]) * (right - left) / (arr[right] - arr[left]));
            
            if (std::abs(arr[pos] - target) < epsilon) {
                return pos;
            } else if (arr[pos] < target) {
                left = pos + 1;
            } else {
                right = pos - 1;
            }
        }
        
        return static_cast<size_t>(-1);
    }
    
    // Interpolation search with custom key function
    template<typename KeyFunc>
    static size_t search_with_key(const std::vector<T>& arr, const T& target, KeyFunc key_func) {
        size_t left = 0;
        size_t right = arr.size() - 1;
        
        while (left <= right && key_func(target) >= key_func(arr[left]) && key_func(target) <= key_func(arr[right])) {
            if (left == right) {
                if (key_func(arr[left]) == key_func(target)) return left;
                return static_cast<size_t>(-1);
            }
            
            // Interpolation formula with key function
            size_t pos = left + static_cast<size_t>((key_func(target) - key_func(arr[left])) * (right - left) / (key_func(arr[right]) - key_func(arr[left])));
            
            if (key_func(arr[pos]) == key_func(target)) {
                return pos;
            } else if (key_func(arr[pos]) < key_func(target)) {
                left = pos + 1;
            } else {
                right = pos - 1;
            }
        }
        
        return static_cast<size_t>(-1);
    }
    
    // Interpolation search for finding insertion point
    static size_t find_insertion_point(const std::vector<T>& arr, const T& target) {
        size_t left = 0;
        size_t right = arr.size() - 1;
        
        while (left <= right && target >= arr[left] && target <= arr[right]) {
            if (left == right) {
                return (arr[left] < target) ? left + 1 : left;
            }
            
            // Interpolation formula
            size_t pos = left + static_cast<size_t>((target - arr[left]) * (right - left) / (arr[right] - arr[left]));
            
            if (arr[pos] < target) {
                left = pos + 1;
            } else {
                right = pos - 1;
            }
        }
        
        return left;
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
    
    // Check if array is suitable for interpolation search
    static bool is_suitable_for_interpolation(const std::vector<T>& arr) {
        if (arr.size() < 3) return true;
        
        // Check if distribution is roughly uniform
        T min_val = arr.front();
        T max_val = arr.back();
        T range = max_val - min_val;
        
        if (range == 0) return true;
        
        // Check if gaps are roughly uniform
        for (size_t i = 1; i < arr.size(); ++i) {
            T gap = arr[i] - arr[i - 1];
            if (gap > range / (arr.size() - 1) * 2) {
                return false;
            }
        }
        
        return true;
    }
    
    // Calculate interpolation search complexity
    static double get_complexity(size_t array_size) {
        if (array_size == 0) return 0.0;
        return std::log2(std::log2(array_size));
    }

private:
    // Binary search fallback
    static size_t binary_search_fallback(const std::vector<T>& arr, const T& target, 
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
};

// Type aliases for common use cases
using IntInterpolationSearch = InterpolationSearch<int>;
using DoubleInterpolationSearch = InterpolationSearch<double>;
using FloatInterpolationSearch = InterpolationSearch<float>;

} // namespace algorithms
} // namespace dsa
