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
 * @brief Search Algorithms Collection
 */
template<typename T>
class Search {
public:
    // Linear Search - O(n)
    static size_t linear_search(const std::vector<T>& arr, const T& target) {
        for (size_t i = 0; i < arr.size(); ++i) {
            if (arr[i] == target) {
                return i;
            }
        }
        return static_cast<size_t>(-1); // Not found
    }
    
    // Binary Search - O(log n) - requires sorted array
    static size_t binary_search(const std::vector<T>& arr, const T& target) {
        size_t left = 0;
        size_t right = arr.size() - 1;
        
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
        
        return static_cast<size_t>(-1); // Not found
    }
    
    // Binary Search (Recursive) - O(log n)
    static size_t binary_search_recursive(const std::vector<T>& arr, const T& target) {
        return binary_search_recursive_helper(arr, target, 0, arr.size() - 1);
    }
    
    // Jump Search - O(√n)
    static size_t jump_search(const std::vector<T>& arr, const T& target) {
        size_t n = arr.size();
        if (n == 0) return static_cast<size_t>(-1);
        
        size_t step = static_cast<size_t>(std::sqrt(n));
        size_t prev = 0;
        
        // Finding the block where element is present
        while (prev < n && arr[std::min(step, n) - 1] < target) {
            prev = step;
            step += static_cast<size_t>(std::sqrt(n));
            if (prev >= n) return static_cast<size_t>(-1);
        }
        
        // Linear search in the identified block
        while (prev < std::min(step, n)) {
            if (arr[prev] == target) {
                return prev;
            }
            ++prev;
        }
        
        return static_cast<size_t>(-1); // Not found
    }
    
    // Interpolation Search - O(log log n) average, O(n) worst
    static size_t interpolation_search(const std::vector<T>& arr, const T& target) {
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
        
        return static_cast<size_t>(-1); // Not found
    }
    
    // Exponential Search - O(log n)
    static size_t exponential_search(const std::vector<T>& arr, const T& target) {
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
    
    // Ternary Search - O(log₃ n)
    static size_t ternary_search(const std::vector<T>& arr, const T& target) {
        return ternary_search_recursive(arr, target, 0, arr.size() - 1);
    }
    
    // Fibonacci Search - O(log n)
    static size_t fibonacci_search(const std::vector<T>& arr, const T& target) {
        size_t n = arr.size();
        if (n == 0) return static_cast<size_t>(-1);
        
        // Initialize Fibonacci numbers
        size_t fib2 = 0;   // (k-2)'th Fibonacci number
        size_t fib1 = 1;   // (k-1)'th Fibonacci number
        size_t fib = fib1 + fib2; // k'th Fibonacci number
        
        // Find the smallest Fibonacci number greater than or equal to n
        while (fib < n) {
            fib2 = fib1;
            fib1 = fib;
            fib = fib1 + fib2;
        }
        
        // Initialize variables for the eliminated range
        size_t offset = -1;
        
        while (fib > 1) {
            // Check if fib2 is a valid index
            size_t i = std::min(offset + fib2, n - 1);
            
            if (arr[i] < target) {
                fib = fib1;
                fib1 = fib2;
                fib2 = fib - fib1;
                offset = i;
            } else if (arr[i] > target) {
                fib = fib2;
                fib1 = fib1 - fib2;
                fib2 = fib - fib1;
            } else {
                return i;
            }
        }
        
        // Compare last element
        if (fib1 && offset + 1 < n && arr[offset + 1] == target) {
            return offset + 1;
        }
        
        return static_cast<size_t>(-1); // Not found
    }
    
    // Meta Binary Search (One-Sided Binary Search) - O(log n)
    static size_t meta_binary_search(const std::vector<T>& arr, const T& target) {
        size_t n = arr.size();
        if (n == 0) return static_cast<size_t>(-1);
        
        size_t lg = static_cast<size_t>(std::log2(n - 1)) + 1;
        size_t pos = 0;
        
        for (size_t i = lg; i >= 0; --i) {
            if (pos + (1 << i) < n && arr[pos + (1 << i)] <= target) {
                pos += (1 << i);
            }
        }
        
        if (pos < n && arr[pos] == target) {
            return pos;
        }
        
        return static_cast<size_t>(-1); // Not found
    }
    
    // Sentinel Linear Search - O(n) but with fewer comparisons
    static size_t sentinel_linear_search(std::vector<T>& arr, const T& target) {
        size_t n = arr.size();
        if (n == 0) return static_cast<size_t>(-1);
        
        // Store the last element
        T last = arr[n - 1];
        
        // Set the last element as sentinel
        arr[n - 1] = target;
        
        size_t i = 0;
        while (arr[i] != target) {
            ++i;
        }
        
        // Restore the last element
        arr[n - 1] = last;
        
        if (i < n - 1 || arr[n - 1] == target) {
            return i;
        }
        
        return static_cast<size_t>(-1); // Not found
    }
    
    // Utility functions
    static bool contains(const std::vector<T>& arr, const T& target) {
        return linear_search(arr, target) != static_cast<size_t>(-1);
    }
    
    static size_t count_occurrences(const std::vector<T>& arr, const T& target) {
        size_t count = 0;
        for (const auto& element : arr) {
            if (element == target) {
                ++count;
            }
        }
        return count;
    }
    
    static std::vector<size_t> find_all_occurrences(const std::vector<T>& arr, const T& target) {
        std::vector<size_t> occurrences;
        for (size_t i = 0; i < arr.size(); ++i) {
            if (arr[i] == target) {
                occurrences.push_back(i);
            }
        }
        return occurrences;
    }
    
    // Custom comparator versions
    template<typename Compare>
    static size_t linear_search(const std::vector<T>& arr, const T& target, Compare comp) {
        for (size_t i = 0; i < arr.size(); ++i) {
            if (!comp(arr[i], target) && !comp(target, arr[i])) {
                return i;
            }
        }
        return static_cast<size_t>(-1);
    }
    
    template<typename Compare>
    static size_t binary_search(const std::vector<T>& arr, const T& target, Compare comp) {
        size_t left = 0;
        size_t right = arr.size() - 1;
        
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
    
    template<typename Compare>
    static bool contains(const std::vector<T>& arr, const T& target, Compare comp) {
        return linear_search(arr, target, comp) != static_cast<size_t>(-1);
    }
    
    // Search with custom equality
    template<typename Equal>
    static size_t linear_search_custom(const std::vector<T>& arr, const T& target, Equal equal) {
        for (size_t i = 0; i < arr.size(); ++i) {
            if (equal(arr[i], target)) {
                return i;
            }
        }
        return static_cast<size_t>(-1);
    }
    
    // Range search functions
    static std::pair<size_t, size_t> find_range(const std::vector<T>& arr, const T& target) {
        size_t first = find_first_occurrence(arr, target);
        if (first == static_cast<size_t>(-1)) {
            return {static_cast<size_t>(-1), static_cast<size_t>(-1)};
        }
        
        size_t last = find_last_occurrence(arr, target);
        return {first, last};
    }
    
    static size_t find_first_occurrence(const std::vector<T>& arr, const T& target) {
        size_t left = 0;
        size_t right = arr.size() - 1;
        size_t result = static_cast<size_t>(-1);
        
        while (left <= right) {
            size_t mid = left + (right - left) / 2;
            
            if (arr[mid] == target) {
                result = mid;
                right = mid - 1; // Continue searching in left half
            } else if (arr[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        return result;
    }
    
    static size_t find_last_occurrence(const std::vector<T>& arr, const T& target) {
        size_t left = 0;
        size_t right = arr.size() - 1;
        size_t result = static_cast<size_t>(-1);
        
        while (left <= right) {
            size_t mid = left + (right - left) / 2;
            
            if (arr[mid] == target) {
                result = mid;
                left = mid + 1; // Continue searching in right half
            } else if (arr[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        return result;
    }

private:
    // Helper functions
    static size_t binary_search_recursive_helper(const std::vector<T>& arr, const T& target, 
                                               size_t left, size_t right) {
        if (left > right) {
            return static_cast<size_t>(-1);
        }
        
        size_t mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            return binary_search_recursive_helper(arr, target, mid + 1, right);
        } else {
            return binary_search_recursive_helper(arr, target, left, mid - 1);
        }
    }
    
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
    
    static size_t ternary_search_recursive(const std::vector<T>& arr, const T& target, 
                                         size_t left, size_t right) {
        if (left > right) {
            return static_cast<size_t>(-1);
        }
        
        size_t mid1 = left + (right - left) / 3;
        size_t mid2 = right - (right - left) / 3;
        
        if (arr[mid1] == target) {
            return mid1;
        }
        
        if (arr[mid2] == target) {
            return mid2;
        }
        
        if (target < arr[mid1]) {
            return ternary_search_recursive(arr, target, left, mid1 - 1);
        } else if (target > arr[mid2]) {
            return ternary_search_recursive(arr, target, mid2 + 1, right);
        } else {
            return ternary_search_recursive(arr, target, mid1 + 1, mid2 - 1);
        }
    }
};

// Type aliases for common use cases
using IntSearch = Search<int>;
using DoubleSearch = Search<double>;
using StringSearch = Search<std::string>;

} // namespace algorithms
} // namespace dsa
