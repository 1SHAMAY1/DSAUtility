#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include <limits>
#include "../Utils/Print.hpp"
#include "../Utils/Logger.hpp"
#include "../Utils/Comparator.hpp"

namespace dsa {
namespace algorithms {

/**
 * @brief Counting Sort Algorithm Implementation
 * Time Complexity: O(n + k) where k is the range of input
 * Space Complexity: O(n + k)
 * Stable: Yes
 */
template<typename T>
class CountSort {
public:
    // Standard counting sort
    static void sort(std::vector<T>& arr) {
        if (arr.empty()) return;
        
        T min_val = *std::min_element(arr.begin(), arr.end());
        T max_val = *std::max_element(arr.begin(), arr.end());
        
        sort_with_range(arr, min_val, max_val);
    }
    
    // Counting sort with specified range
    static void sort_with_range(std::vector<T>& arr, T min_val, T max_val) {
        if (arr.empty()) return;
        
        size_t range = max_val - min_val + 1;
        std::vector<size_t> count(range, 0);
        std::vector<T> output(arr.size());
        
        // Count occurrences
        for (const auto& val : arr) {
            count[val - min_val]++;
        }
        
        // Calculate positions
        for (size_t i = 1; i < range; ++i) {
            count[i] += count[i - 1];
        }
        
        // Build output array
        for (int i = arr.size() - 1; i >= 0; --i) {
            output[count[arr[i] - min_val] - 1] = arr[i];
            count[arr[i] - min_val]--;
        }
        
        arr = output;
    }
    
    // Counting sort with custom key function
    template<typename KeyFunc>
    static void sort_with_key(std::vector<T>& arr, KeyFunc key_func) {
        if (arr.empty()) return;
        
        // Find min and max keys
        auto min_key = key_func(arr[0]);
        auto max_key = key_func(arr[0]);
        
        for (const auto& val : arr) {
            auto key = key_func(val);
            min_key = std::min(min_key, key);
            max_key = std::max(max_key, key);
        }
        
        size_t range = max_key - min_key + 1;
        std::vector<size_t> count(range, 0);
        std::vector<T> output(arr.size());
        
        // Count occurrences
        for (const auto& val : arr) {
            count[key_func(val) - min_key]++;
        }
        
        // Calculate positions
        for (size_t i = 1; i < range; ++i) {
            count[i] += count[i - 1];
        }
        
        // Build output array
        for (int i = arr.size() - 1; i >= 0; --i) {
            output[count[key_func(arr[i]) - min_key] - 1] = arr[i];
            count[key_func(arr[i]) - min_key]--;
        }
        
        arr = output;
    }
    
    // In-place counting sort (modifies input array)
    static void in_place_sort(std::vector<T>& arr) {
        if (arr.empty()) return;
        
        T min_val = *std::min_element(arr.begin(), arr.end());
        T max_val = *std::max_element(arr.begin(), arr.end());
        
        size_t range = max_val - min_val + 1;
        std::vector<size_t> count(range, 0);
        
        // Count occurrences
        for (const auto& val : arr) {
            count[val - min_val]++;
        }
        
        // Reconstruct array
        size_t index = 0;
        for (size_t i = 0; i < range; ++i) {
            for (size_t j = 0; j < count[i]; ++j) {
                arr[index++] = min_val + i;
            }
        }
    }
    
    // Counting sort with custom comparator
    template<typename Compare>
    static void sort_with_comparator(std::vector<T>& arr, Compare comp) {
        if (arr.empty()) return;
        
        T min_val = arr[0];
        T max_val = arr[0];
        
        for (const auto& val : arr) {
            if (comp(val, min_val)) min_val = val;
            if (comp(max_val, val)) max_val = val;
        }
        
        sort_with_range(arr, min_val, max_val);
    }
    
    // Utility functions
    static bool is_sorted(const std::vector<T>& arr) {
        return std::is_sorted(arr.begin(), arr.end());
    }
    
    static size_t get_range(const std::vector<T>& arr) {
        if (arr.empty()) return 0;
        
        T min_val = *std::min_element(arr.begin(), arr.end());
        T max_val = *std::max_element(arr.begin(), arr.end());
        
        return max_val - min_val + 1;
    }
    
    static std::vector<size_t> get_frequency(const std::vector<T>& arr) {
        if (arr.empty()) return {};
        
        T min_val = *std::min_element(arr.begin(), arr.end());
        T max_val = *std::max_element(arr.begin(), arr.end());
        
        size_t range = max_val - min_val + 1;
        std::vector<size_t> frequency(range, 0);
        
        for (const auto& val : arr) {
            frequency[val - min_val]++;
        }
        
        return frequency;
    }
    
    // Find k-th smallest element using counting sort
    static T kth_smallest(const std::vector<T>& arr, size_t k) {
        if (k >= arr.size()) {
            throw std::out_of_range("k is out of range");
        }
        
        T min_val = *std::min_element(arr.begin(), arr.end());
        T max_val = *std::max_element(arr.begin(), arr.end());
        
        size_t range = max_val - min_val + 1;
        std::vector<size_t> count(range, 0);
        
        // Count occurrences
        for (const auto& val : arr) {
            count[val - min_val]++;
        }
        
        // Find k-th smallest
        size_t cumulative = 0;
        for (size_t i = 0; i < range; ++i) {
            cumulative += count[i];
            if (cumulative > k) {
                return min_val + i;
            }
        }
        
        return max_val; // Fallback
    }
    
    // Find k-th largest element using counting sort
    static T kth_largest(const std::vector<T>& arr, size_t k) {
        if (k >= arr.size()) {
            throw std::out_of_range("k is out of range");
        }
        
        return kth_smallest(arr, arr.size() - 1 - k);
    }
    
    // Remove duplicates while sorting
    static std::vector<T> sort_unique(std::vector<T>& arr) {
        if (arr.empty()) return {};
        
        T min_val = *std::min_element(arr.begin(), arr.end());
        T max_val = *std::max_element(arr.begin(), arr.end());
        
        size_t range = max_val - min_val + 1;
        std::vector<bool> present(range, false);
        
        // Mark present elements
        for (const auto& val : arr) {
            present[val - min_val] = true;
        }
        
        // Collect unique elements in sorted order
        std::vector<T> result;
        for (size_t i = 0; i < range; ++i) {
            if (present[i]) {
                result.push_back(min_val + i);
            }
        }
        
        return result;
    }
    
    // Sort with stability check
    static bool is_stable_sort(const std::vector<T>& original, const std::vector<T>& sorted) {
        if (original.size() != sorted.size()) return false;
        
        // Create pairs of (value, original_index) for stability check
        std::vector<std::pair<T, size_t>> original_pairs;
        for (size_t i = 0; i < original.size(); ++i) {
            original_pairs.emplace_back(original[i], i);
        }
        
        std::vector<std::pair<T, size_t>> sorted_pairs;
        for (size_t i = 0; i < sorted.size(); ++i) {
            sorted_pairs.emplace_back(sorted[i], i);
        }
        
        // Sort both using counting sort
        sort_pairs(original_pairs);
        sort_pairs(sorted_pairs);
        
        // Check if relative order of equal elements is preserved
        for (size_t i = 0; i < original_pairs.size(); ++i) {
            if (original_pairs[i].first != sorted_pairs[i].first) return false;
        }
        
        return true;
    }

private:
    // Helper function to sort pairs
    static void sort_pairs(std::vector<std::pair<T, size_t>>& pairs) {
        if (pairs.empty()) return;
        
        T min_val = pairs[0].first;
        T max_val = pairs[0].first;
        
        for (const auto& pair : pairs) {
            min_val = std::min(min_val, pair.first);
            max_val = std::max(max_val, pair.first);
        }
        
        size_t range = max_val - min_val + 1;
        std::vector<size_t> count(range, 0);
        std::vector<std::pair<T, size_t>> output(pairs.size());
        
        // Count occurrences
        for (const auto& pair : pairs) {
            count[pair.first - min_val]++;
        }
        
        // Calculate positions
        for (size_t i = 1; i < range; ++i) {
            count[i] += count[i - 1];
        }
        
        // Build output array
        for (int i = pairs.size() - 1; i >= 0; --i) {
            output[count[pairs[i].first - min_val] - 1] = pairs[i];
            count[pairs[i].first - min_val]--;
        }
        
        pairs = output;
    }
};

// Type aliases for common use cases
using IntCountSort = CountSort<int>;
using CharCountSort = CountSort<char>;
using UIntCountSort = CountSort<unsigned int>;

} // namespace algorithms
} // namespace dsa
