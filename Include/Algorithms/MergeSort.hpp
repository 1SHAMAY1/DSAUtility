#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include "../Utils/Print.hpp"
#include "../Utils/Logger.hpp"
#include "../Utils/Comparator.hpp"

namespace dsa {
namespace algorithms {

/**
 * @brief MergeSort Algorithm Implementation
 * Time Complexity: O(n log n)
 * Space Complexity: O(n)
 */
template<typename T>
class MergeSort {
public:
    // Standard merge sort
    static void sort(std::vector<T>& arr) {
        if (arr.size() <= 1) return;
        
        std::vector<T> temp(arr.size());
        merge_sort_recursive(arr, temp, 0, arr.size() - 1);
    }
    
    // Merge sort with custom comparator
    template<typename Compare>
    static void sort(std::vector<T>& arr, Compare comp) {
        if (arr.size() <= 1) return;
        
        std::vector<T> temp(arr.size());
        merge_sort_recursive(arr, temp, 0, arr.size() - 1, comp);
    }
    
    // Bottom-up merge sort (iterative)
    static void bottom_up_sort(std::vector<T>& arr) {
        size_t n = arr.size();
        if (n <= 1) return;
        
        std::vector<T> temp(n);
        
        for (size_t width = 1; width < n; width = 2 * width) {
            for (size_t i = 0; i < n; i = i + 2 * width) {
                size_t left = i;
                size_t mid = std::min(i + width, n);
                size_t right = std::min(i + 2 * width, n);
                
                bottom_up_merge(arr, temp, left, mid, right);
            }
        }
    }
    
    // Natural merge sort (adaptive)
    static void natural_sort(std::vector<T>& arr) {
        size_t n = arr.size();
        if (n <= 1) return;
        
        std::vector<T> temp(n);
        
        while (true) {
            size_t runs = 0;
            size_t i = 0;
            
            while (i < n) {
                size_t start = i;
                
                // Find the end of the current run
                while (i + 1 < n && arr[i] <= arr[i + 1]) {
                    ++i;
                }
                
                size_t end = i + 1;
                
                if (runs % 2 == 0) {
                    // Copy run to temp array
                    for (size_t j = start; j < end; ++j) {
                        temp[j] = arr[j];
                    }
                } else {
                    // Merge with previous run
                    natural_merge(arr, temp, start, end);
                }
                
                ++runs;
                ++i;
            }
            
            // If only one run remains, we're done
            if (runs <= 1) break;
        }
    }
    
    // In-place merge sort (space-optimized)
    static void in_place_sort(std::vector<T>& arr) {
        if (arr.size() <= 1) return;
        in_place_merge_sort_recursive(arr, 0, arr.size() - 1);
    }
    
    // Block merge sort (cache-friendly)
    static void block_sort(std::vector<T>& arr) {
        size_t n = arr.size();
        if (n <= 1) return;
        
        const size_t BLOCK_SIZE = 32; // Cache-friendly block size
        std::vector<T> temp(n);
        
        // Sort blocks using insertion sort
        for (size_t i = 0; i < n; i += BLOCK_SIZE) {
            size_t end = std::min(i + BLOCK_SIZE, n);
            insertion_sort_range(arr, i, end - 1);
        }
        
        // Merge blocks
        for (size_t width = BLOCK_SIZE; width < n; width = 2 * width) {
            for (size_t i = 0; i < n; i = i + 2 * width) {
                size_t left = i;
                size_t mid = std::min(i + width, n);
                size_t right = std::min(i + 2 * width, n);
                
                block_merge(arr, temp, left, mid, right);
            }
        }
    }
    
    // Utility functions
    static bool is_sorted(const std::vector<T>& arr) {
        return std::is_sorted(arr.begin(), arr.end());
    }
    
    static size_t inversion_count(const std::vector<T>& arr) {
        return inversion_count_recursive(arr, 0, arr.size() - 1);
    }
    
    // Get merge count for performance analysis
    static size_t merge_count(const std::vector<T>& arr) {
        std::vector<T> temp(arr.size());
        return merge_count_recursive(arr, temp, 0, arr.size() - 1);
    }

private:
    // Standard merge sort implementation
    static void merge_sort_recursive(std::vector<T>& arr, std::vector<T>& temp, 
                                   size_t left, size_t right) {
        if (left < right) {
            size_t mid = left + (right - left) / 2;
            merge_sort_recursive(arr, temp, left, mid);
            merge_sort_recursive(arr, temp, mid + 1, right);
            merge(arr, temp, left, mid, right);
        }
    }
    
    template<typename Compare>
    static void merge_sort_recursive(std::vector<T>& arr, std::vector<T>& temp, 
                                   size_t left, size_t right, Compare comp) {
        if (left < right) {
            size_t mid = left + (right - left) / 2;
            merge_sort_recursive(arr, temp, left, mid, comp);
            merge_sort_recursive(arr, temp, mid + 1, right, comp);
            merge(arr, temp, left, mid, right, comp);
        }
    }
    
    // Standard merge function
    static void merge(std::vector<T>& arr, std::vector<T>& temp, 
                     size_t left, size_t mid, size_t right) {
        size_t i = left, j = mid + 1, k = left;
        
        while (i <= mid && j <= right) {
            if (arr[i] <= arr[j]) {
                temp[k++] = arr[i++];
            } else {
                temp[k++] = arr[j++];
            }
        }
        
        while (i <= mid) {
            temp[k++] = arr[i++];
        }
        
        while (j <= right) {
            temp[k++] = arr[j++];
        }
        
        for (i = left; i <= right; ++i) {
            arr[i] = temp[i];
        }
    }
    
    template<typename Compare>
    static void merge(std::vector<T>& arr, std::vector<T>& temp, 
                     size_t left, size_t mid, size_t right, Compare comp) {
        size_t i = left, j = mid + 1, k = left;
        
        while (i <= mid && j <= right) {
            if (comp(arr[i], arr[j]) || arr[i] == arr[j]) {
                temp[k++] = arr[i++];
            } else {
                temp[k++] = arr[j++];
            }
        }
        
        while (i <= mid) {
            temp[k++] = arr[i++];
        }
        
        while (j <= right) {
            temp[k++] = arr[j++];
        }
        
        for (i = left; i <= right; ++i) {
            arr[i] = temp[i];
        }
    }
    
    // Bottom-up merge function
    static void bottom_up_merge(std::vector<T>& arr, std::vector<T>& temp, 
                               size_t left, size_t mid, size_t right) {
        size_t i = left, j = mid, k = left;
        
        while (i < mid && j < right) {
            if (arr[i] <= arr[j]) {
                temp[k++] = arr[i++];
            } else {
                temp[k++] = arr[j++];
            }
        }
        
        while (i < mid) {
            temp[k++] = arr[i++];
        }
        
        while (j < right) {
            temp[k++] = arr[j++];
        }
        
        for (i = left; i < right; ++i) {
            arr[i] = temp[i];
        }
    }
    
    // Natural merge function
    static void natural_merge(std::vector<T>& arr, std::vector<T>& temp, 
                             size_t start, size_t end) {
        size_t i = start, j = end, k = start;
        
        while (i < end && j < arr.size() && arr[j] <= arr[j + 1]) {
            if (temp[i] <= arr[j]) {
                arr[k++] = temp[i++];
            } else {
                arr[k++] = arr[j++];
            }
        }
        
        while (i < end) {
            arr[k++] = temp[i++];
        }
    }
    
    // In-place merge sort implementation
    static void in_place_merge_sort_recursive(std::vector<T>& arr, size_t left, size_t right) {
        if (left < right) {
            size_t mid = left + (right - left) / 2;
            in_place_merge_sort_recursive(arr, left, mid);
            in_place_merge_sort_recursive(arr, mid + 1, right);
            in_place_merge(arr, left, mid, right);
        }
    }
    
    static void in_place_merge(std::vector<T>& arr, size_t left, size_t mid, size_t right) {
        size_t i = left, j = mid + 1;
        
        while (i <= mid && j <= right) {
            if (arr[i] <= arr[j]) {
                ++i;
            } else {
                T temp = arr[j];
                size_t k = j;
                
                while (k > i) {
                    arr[k] = arr[k - 1];
                    --k;
                }
                
                arr[i] = temp;
                ++i;
                ++mid;
                ++j;
            }
        }
    }
    
    // Block merge function
    static void block_merge(std::vector<T>& arr, std::vector<T>& temp, 
                           size_t left, size_t mid, size_t right) {
        size_t i = left, j = mid, k = left;
        
        while (i < mid && j < right) {
            if (arr[i] <= arr[j]) {
                temp[k++] = arr[i++];
            } else {
                temp[k++] = arr[j++];
            }
        }
        
        while (i < mid) {
            temp[k++] = arr[i++];
        }
        
        while (j < right) {
            temp[k++] = arr[j++];
        }
        
        for (i = left; i < right; ++i) {
            arr[i] = temp[i];
        }
    }
    
    // Insertion sort for small ranges
    static void insertion_sort_range(std::vector<T>& arr, size_t left, size_t right) {
        for (size_t i = left + 1; i <= right; ++i) {
            T key = arr[i];
            size_t j = i;
            
            while (j > left && arr[j - 1] > key) {
                arr[j] = arr[j - 1];
                --j;
            }
            arr[j] = key;
        }
    }
    
    // Inversion count calculation
    static size_t inversion_count_recursive(const std::vector<T>& arr, size_t left, size_t right) {
        size_t count = 0;
        
        if (left < right) {
            size_t mid = left + (right - left) / 2;
            count += inversion_count_recursive(arr, left, mid);
            count += inversion_count_recursive(arr, mid + 1, right);
            count += merge_inversion_count(arr, left, mid, right);
        }
        
        return count;
    }
    
    static size_t merge_inversion_count(const std::vector<T>& arr, size_t left, size_t mid, size_t right) {
        std::vector<T> temp(right - left + 1);
        size_t i = left, j = mid + 1, k = 0;
        size_t count = 0;
        
        while (i <= mid && j <= right) {
            if (arr[i] <= arr[j]) {
                temp[k++] = arr[i++];
            } else {
                temp[k++] = arr[j++];
                count += mid - i + 1;
            }
        }
        
        while (i <= mid) {
            temp[k++] = arr[i++];
        }
        
        while (j <= right) {
            temp[k++] = arr[j++];
        }
        
        return count;
    }
    
    // Merge count calculation
    static size_t merge_count_recursive(std::vector<T>& arr, std::vector<T>& temp, 
                                       size_t left, size_t right) {
        size_t count = 0;
        
        if (left < right) {
            size_t mid = left + (right - left) / 2;
            count += merge_count_recursive(arr, temp, left, mid);
            count += merge_count_recursive(arr, temp, mid + 1, right);
            merge(arr, temp, left, mid, right);
            ++count;
        }
        
        return count;
    }
};

// Type aliases for common use cases
using IntMergeSort = MergeSort<int>;
using DoubleMergeSort = MergeSort<double>;
using StringMergeSort = MergeSort<std::string>;

} // namespace algorithms
} // namespace dsa
