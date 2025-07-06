#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include <random>
#include "../Utils/Print.hpp"
#include "../Utils/Logger.hpp"
#include "../Utils/Comparator.hpp"
#include "../Utils/Swap.hpp"

namespace dsa {
namespace algorithms {

/**
 * @brief QuickSort Algorithm Implementation
 * Time Complexity: O(n log n) average, O(n²) worst case
 * Space Complexity: O(log n) average, O(n) worst case
 */
template<typename T>
class QuickSort {
public:
    enum class PivotStrategy {
        LAST,      // Use last element as pivot
        FIRST,     // Use first element as pivot
        MIDDLE,    // Use middle element as pivot
        RANDOM,    // Use random element as pivot
        MEDIAN_OF_THREE, // Use median of first, middle, last
        NINTHELEMENT // Use nth element as pivot
    };
    
    // Main quick sort function
    static void sort(std::vector<T>& arr, PivotStrategy strategy = PivotStrategy::RANDOM) {
        if (arr.size() <= 1) return;
        quick_sort_recursive(arr, 0, arr.size() - 1, strategy);
    }
    
    // Quick sort with custom comparator
    template<typename Compare>
    static void sort(std::vector<T>& arr, Compare comp, PivotStrategy strategy = PivotStrategy::RANDOM) {
        if (arr.size() <= 1) return;
        quick_sort_recursive(arr, 0, arr.size() - 1, comp, strategy);
    }
    
    // Three-way partition quick sort (handles duplicates efficiently)
    static void three_way_sort(std::vector<T>& arr) {
        if (arr.size() <= 1) return;
        three_way_quick_sort_recursive(arr, 0, arr.size() - 1);
    }
    
    // Dual pivot quick sort (faster for arrays with many duplicates)
    static void dual_pivot_sort(std::vector<T>& arr) {
        if (arr.size() <= 1) return;
        dual_pivot_quick_sort_recursive(arr, 0, arr.size() - 1);
    }
    
    // Tail call optimized quick sort
    static void tail_call_optimized_sort(std::vector<T>& arr) {
        if (arr.size() <= 1) return;
        tail_call_quick_sort_recursive(arr, 0, arr.size() - 1);
    }
    
    // Utility functions
    static bool is_sorted(const std::vector<T>& arr) {
        return std::is_sorted(arr.begin(), arr.end());
    }
    
    static size_t partition_count(const std::vector<T>& arr) {
        return partition_count_recursive(arr, 0, arr.size() - 1);
    }
    
    // Get pivot element based on strategy
    static T get_pivot(const std::vector<T>& arr, size_t low, size_t high, PivotStrategy strategy) {
        switch (strategy) {
            case PivotStrategy::FIRST:
                return arr[low];
            case PivotStrategy::LAST:
                return arr[high];
            case PivotStrategy::MIDDLE:
                return arr[low + (high - low) / 2];
            case PivotStrategy::RANDOM:
                return arr[get_random_index(low, high)];
            case PivotStrategy::MEDIAN_OF_THREE:
                return median_of_three(arr, low, high);
            case PivotStrategy::NINTHELEMENT:
                return arr[low + (high - low) / 9]; // 9th element
            default:
                return arr[high];
        }
    }

private:
    // Standard quick sort implementation
    static void quick_sort_recursive(std::vector<T>& arr, size_t low, size_t high, PivotStrategy strategy) {
        if (low < high) {
            size_t pi = partition(arr, low, high, strategy);
            if (pi > 0) quick_sort_recursive(arr, low, pi - 1, strategy);
            quick_sort_recursive(arr, pi + 1, high, strategy);
        }
    }
    
    template<typename Compare>
    static void quick_sort_recursive(std::vector<T>& arr, size_t low, size_t high, 
                                   Compare comp, PivotStrategy strategy) {
        if (low < high) {
            size_t pi = partition(arr, low, high, comp, strategy);
            if (pi > 0) quick_sort_recursive(arr, low, pi - 1, comp, strategy);
            quick_sort_recursive(arr, pi + 1, high, comp, strategy);
        }
    }
    
    // Standard partition function
    static size_t partition(std::vector<T>& arr, size_t low, size_t high, PivotStrategy strategy) {
        T pivot = get_pivot(arr, low, high, strategy);
        size_t i = low;
        
        for (size_t j = low; j < high; ++j) {
            if (arr[j] <= pivot) {
                utils::Swap::swap(arr[i], arr[j]);
                ++i;
            }
        }
        utils::Swap::swap(arr[i], arr[high]);
        return i;
    }
    
    template<typename Compare>
    static size_t partition(std::vector<T>& arr, size_t low, size_t high, 
                          Compare comp, PivotStrategy strategy) {
        T pivot = get_pivot(arr, low, high, strategy);
        size_t i = low;
        
        for (size_t j = low; j < high; ++j) {
            if (comp(arr[j], pivot) || arr[j] == pivot) {
                utils::Swap::swap(arr[i], arr[j]);
                ++i;
            }
        }
        utils::Swap::swap(arr[i], arr[high]);
        return i;
    }
    
    // Three-way partition quick sort
    static void three_way_quick_sort_recursive(std::vector<T>& arr, size_t low, size_t high) {
        if (low < high) {
            std::pair<size_t, size_t> result = three_way_partition(arr, low, high);
            size_t lt = result.first;
            size_t gt = result.second;
            three_way_quick_sort_recursive(arr, low, lt - 1);
            three_way_quick_sort_recursive(arr, gt + 1, high);
        }
    }
    
    static std::pair<size_t, size_t> three_way_partition(std::vector<T>& arr, size_t low, size_t high) {
        T pivot = arr[high];
        size_t lt = low;     // Elements less than pivot
        size_t gt = high;    // Elements greater than pivot
        size_t i = low;      // Current element
        
        while (i <= gt) {
            if (arr[i] < pivot) {
                utils::Swap::swap(arr[lt], arr[i]);
                ++lt;
                ++i;
            } else if (arr[i] > pivot) {
                utils::Swap::swap(arr[i], arr[gt]);
                --gt;
            } else {
                ++i;
            }
        }
        
        return {lt, gt};
    }
    
    // Dual pivot quick sort
    static void dual_pivot_quick_sort_recursive(std::vector<T>& arr, size_t low, size_t high) {
        if (low < high) {
            if (arr[low] > arr[high]) {
                utils::Swap::swap(arr[low], arr[high]);
            }
            
            T pivot1 = arr[low];
            T pivot2 = arr[high];
            
            size_t lt = low + 1;   // Elements less than pivot1
            size_t gt = high - 1;  // Elements greater than pivot2
            size_t i = low + 1;    // Current element
            
            while (i <= gt) {
                if (arr[i] < pivot1) {
                    utils::Swap::swap(arr[lt], arr[i]);
                    ++lt;
                    ++i;
                } else if (arr[i] > pivot2) {
                    utils::Swap::swap(arr[i], arr[gt]);
                    --gt;
                } else {
                    ++i;
                }
            }
            
            utils::Swap::swap(arr[low], arr[lt - 1]);
            utils::Swap::swap(arr[high], arr[gt + 1]);
            
            dual_pivot_quick_sort_recursive(arr, low, lt - 2);
            dual_pivot_quick_sort_recursive(arr, lt, gt);
            dual_pivot_quick_sort_recursive(arr, gt + 2, high);
        }
    }
    
    // Tail call optimized quick sort
    static void tail_call_quick_sort_recursive(std::vector<T>& arr, size_t low, size_t high) {
        while (low < high) {
            size_t pi = partition(arr, low, high, PivotStrategy::RANDOM);
            
            // Sort the smaller partition first
            if (pi - low < high - pi) {
                tail_call_quick_sort_recursive(arr, low, pi - 1);
                low = pi + 1;
            } else {
                tail_call_quick_sort_recursive(arr, pi + 1, high);
                high = pi - 1;
            }
        }
    }
    
    // Helper functions
    static size_t get_random_index(size_t low, size_t high) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dis(low, high);
        return dis(gen);
    }
    
    static T median_of_three(const std::vector<T>& arr, size_t low, size_t high) {
        size_t mid = low + (high - low) / 2;
        
        T a = arr[low];
        T b = arr[mid];
        T c = arr[high];
        
        if (a > b) {
            if (b > c) return b;
            else if (a > c) return c;
            else return a;
        } else {
            if (a > c) return a;
            else if (b > c) return c;
            else return b;
        }
    }
    
    static size_t partition_count_recursive(const std::vector<T>& arr, size_t low, size_t high) {
        if (low >= high) return 0;
        
        size_t count = 1; // Current partition
        size_t pi = partition_count_helper(arr, low, high);
        
        if (pi > 0) count += partition_count_recursive(arr, low, pi - 1);
        count += partition_count_recursive(arr, pi + 1, high);
        
        return count;
    }
    
    static size_t partition_count_helper(const std::vector<T>& arr, size_t low, size_t high) {
        T pivot = arr[high];
        size_t i = low;
        
        for (size_t j = low; j < high; ++j) {
            if (arr[j] <= pivot) {
                ++i;
            }
        }
        
        return i;
    }
};

// Type aliases for common use cases
using IntQuickSort = QuickSort<int>;
using DoubleQuickSort = QuickSort<double>;
using StringQuickSort = QuickSort<std::string>;

} // namespace algorithms
} // namespace dsa
