#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include <random>
#include <chrono>
#include "../Utils/Print.hpp"
#include "../Utils/Logger.hpp"
#include "../Utils/Comparator.hpp"
#include "../Utils/Swap.hpp"

namespace dsa {
namespace algorithms {

/**
 * @brief Sorting Algorithms Collection
 */
template<typename T>
class Sort {
public:
    // Bubble Sort - O(n²)
    static void bubble_sort(std::vector<T>& arr) {
        size_t n = arr.size();
        bool swapped;
        
        for (size_t i = 0; i < n - 1; ++i) {
            swapped = false;
            for (size_t j = 0; j < n - i - 1; ++j) {
                if (arr[j] > arr[j + 1]) {
                    utils::Swap::swap(arr[j], arr[j + 1]);
                    swapped = true;
                }
            }
            if (!swapped) break; // Array is sorted
        }
    }
    
    // Selection Sort - O(n²)
    static void selection_sort(std::vector<T>& arr) {
        size_t n = arr.size();
        
        for (size_t i = 0; i < n - 1; ++i) {
            size_t min_idx = i;
            for (size_t j = i + 1; j < n; ++j) {
                if (arr[j] < arr[min_idx]) {
                    min_idx = j;
                }
            }
            if (min_idx != i) {
                utils::Swap::swap(arr[i], arr[min_idx]);
            }
        }
    }
    
    // Insertion Sort - O(n²)
    static void insertion_sort(std::vector<T>& arr) {
        size_t n = arr.size();
        
        for (size_t i = 1; i < n; ++i) {
            T key = arr[i];
            size_t j = i;
            
            while (j > 0 && arr[j - 1] > key) {
                arr[j] = arr[j - 1];
                --j;
            }
            arr[j] = key;
        }
    }
    
    // Quick Sort - O(n log n) average, O(n²) worst
    static void quick_sort(std::vector<T>& arr) {
        quick_sort_recursive(arr, 0, arr.size() - 1);
    }
    
    // Merge Sort - O(n log n)
    static void merge_sort(std::vector<T>& arr) {
        if (arr.size() <= 1) return;
        
        std::vector<T> temp(arr.size());
        merge_sort_recursive(arr, temp, 0, arr.size() - 1);
    }
    
    // Heap Sort - O(n log n)
    static void heap_sort(std::vector<T>& arr) {
        size_t n = arr.size();
        
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
    
    // Counting Sort - O(n + k) where k is the range
    static void counting_sort(std::vector<T>& arr) {
        if (arr.empty()) return;
        
        T min_val = *std::min_element(arr.begin(), arr.end());
        T max_val = *std::max_element(arr.begin(), arr.end());
        
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
    
    // Radix Sort - O(d * (n + k)) where d is number of digits
    static void radix_sort(std::vector<T>& arr) {
        if (arr.empty()) return;
        
        T max_val = *std::max_element(arr.begin(), arr.end());
        
        for (T exp = 1; max_val / exp > 0; exp *= 10) {
            counting_sort_by_digit(arr, exp);
        }
    }
    
    // Shell Sort - O(n log n) to O(n²) depending on gap sequence
    static void shell_sort(std::vector<T>& arr) {
        size_t n = arr.size();
        
        for (size_t gap = n / 2; gap > 0; gap /= 2) {
            for (size_t i = gap; i < n; ++i) {
                T temp = arr[i];
                size_t j;
                
                for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                    arr[j] = arr[j - gap];
                }
                arr[j] = temp;
            }
        }
    }
    
    // Tim Sort - O(n log n)
    static void tim_sort(std::vector<T>& arr) {
        const size_t RUN = 32;
        size_t n = arr.size();
        
        // Sort individual subarrays of size RUN
        for (size_t i = 0; i < n; i += RUN) {
            insertion_sort_range(arr, i, std::min(i + RUN - 1, n - 1));
        }
        
        // Merge subarrays
        for (size_t size = RUN; size < n; size = 2 * size) {
            for (size_t left = 0; left < n; left += 2 * size) {
                size_t mid = std::min(left + size - 1, n - 1);
                size_t right = std::min(left + 2 * size - 1, n - 1);
                
                merge(arr, left, mid, right);
            }
        }
    }
    
    // Intro Sort - O(n log n)
    static void intro_sort(std::vector<T>& arr) {
        size_t n = arr.size();
        int max_depth = 2 * static_cast<int>(std::log2(n));
        intro_sort_recursive(arr, 0, n - 1, max_depth);
    }
    
    // Bucket Sort - O(n + k) average
    static void bucket_sort(std::vector<T>& arr) {
        if (arr.empty()) return;
        
        T min_val = *std::min_element(arr.begin(), arr.end());
        T max_val = *std::max_element(arr.begin(), arr.end());
        
        size_t bucket_count = arr.size();
        std::vector<std::vector<T>> buckets(bucket_count);
        
        // Distribute elements into buckets
        for (const auto& val : arr) {
            size_t bucket_index = static_cast<size_t>((val - min_val) * (bucket_count - 1) / (max_val - min_val));
            buckets[bucket_index].push_back(val);
        }
        
        // Sort individual buckets
        for (auto& bucket : buckets) {
            std::sort(bucket.begin(), bucket.end());
        }
        
        // Concatenate buckets
        size_t index = 0;
        for (const auto& bucket : buckets) {
            for (const auto& val : bucket) {
                arr[index++] = val;
            }
        }
    }
    
    // Utility functions
    static bool is_sorted(const std::vector<T>& arr) {
        return std::is_sorted(arr.begin(), arr.end());
    }
    
    static void shuffle(std::vector<T>& arr) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(arr.begin(), arr.end(), gen);
    }
    
    static void reverse(std::vector<T>& arr) {
        std::reverse(arr.begin(), arr.end());
    }
    
    // Custom comparator versions
    template<typename Compare>
    static void bubble_sort(std::vector<T>& arr, Compare comp) {
        size_t n = arr.size();
        bool swapped;
        
        for (size_t i = 0; i < n - 1; ++i) {
            swapped = false;
            for (size_t j = 0; j < n - i - 1; ++j) {
                if (comp(arr[j + 1], arr[j])) {
                    utils::Swap::swap(arr[j], arr[j + 1]);
                    swapped = true;
                }
            }
            if (!swapped) break;
        }
    }
    
    template<typename Compare>
    static void quick_sort(std::vector<T>& arr, Compare comp) {
        quick_sort_recursive(arr, 0, arr.size() - 1, comp);
    }
    
    template<typename Compare>
    static void merge_sort(std::vector<T>& arr, Compare comp) {
        if (arr.size() <= 1) return;
        
        std::vector<T> temp(arr.size());
        merge_sort_recursive(arr, temp, 0, arr.size() - 1, comp);
    }
    
    template<typename Compare>
    static bool is_sorted(const std::vector<T>& arr, Compare comp) {
        return std::is_sorted(arr.begin(), arr.end(), comp);
    }

private:
    // Helper functions for Quick Sort
    static void quick_sort_recursive(std::vector<T>& arr, size_t low, size_t high) {
        if (low < high) {
            size_t pi = partition(arr, low, high);
            if (pi > 0) quick_sort_recursive(arr, low, pi - 1);
            quick_sort_recursive(arr, pi + 1, high);
        }
    }
    
    static size_t partition(std::vector<T>& arr, size_t low, size_t high) {
        T pivot = arr[high];
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
    static void quick_sort_recursive(std::vector<T>& arr, size_t low, size_t high, Compare comp) {
        if (low < high) {
            size_t pi = partition(arr, low, high, comp);
            if (pi > 0) quick_sort_recursive(arr, low, pi - 1, comp);
            quick_sort_recursive(arr, pi + 1, high, comp);
        }
    }
    
    template<typename Compare>
    static size_t partition(std::vector<T>& arr, size_t low, size_t high, Compare comp) {
        T pivot = arr[high];
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
    
    // Helper functions for Merge Sort
    static void merge_sort_recursive(std::vector<T>& arr, std::vector<T>& temp, 
                                   size_t left, size_t right) {
        if (left < right) {
            size_t mid = left + (right - left) / 2;
            merge_sort_recursive(arr, temp, left, mid);
            merge_sort_recursive(arr, temp, mid + 1, right);
            merge(arr, temp, left, mid, right);
        }
    }
    
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
    static void merge_sort_recursive(std::vector<T>& arr, std::vector<T>& temp, 
                                   size_t left, size_t right, Compare comp) {
        if (left < right) {
            size_t mid = left + (right - left) / 2;
            merge_sort_recursive(arr, temp, left, mid, comp);
            merge_sort_recursive(arr, temp, mid + 1, right, comp);
            merge(arr, temp, left, mid, right, comp);
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
    
    // Helper functions for Heap Sort
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
    
    // Helper functions for Radix Sort
    static void counting_sort_by_digit(std::vector<T>& arr, T exp) {
        std::vector<T> output(arr.size());
        std::vector<int> count(10, 0);
        
        for (const auto& val : arr) {
            count[(val / exp) % 10]++;
        }
        
        for (int i = 1; i < 10; ++i) {
            count[i] += count[i - 1];
        }
        
        for (int i = arr.size() - 1; i >= 0; --i) {
            output[count[(arr[i] / exp) % 10] - 1] = arr[i];
            count[(arr[i] / exp) % 10]--;
        }
        
        arr = output;
    }
    
    // Helper functions for Tim Sort
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
    
    static void merge(std::vector<T>& arr, size_t left, size_t mid, size_t right) {
        std::vector<T> temp(right - left + 1);
        size_t i = left, j = mid + 1, k = 0;
        
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
        
        for (i = 0; i < k; ++i) {
            arr[left + i] = temp[i];
        }
    }
    
    // Helper functions for Intro Sort
    static void intro_sort_recursive(std::vector<T>& arr, size_t low, size_t high, int depth_limit) {
        size_t size = high - low + 1;
        
        if (size < 16) {
            insertion_sort_range(arr, low, high);
        } else if (depth_limit == 0) {
            heap_sort_range(arr, low, high);
        } else {
            size_t pi = partition(arr, low, high);
            if (pi > 0) intro_sort_recursive(arr, low, pi - 1, depth_limit - 1);
            intro_sort_recursive(arr, pi + 1, high, depth_limit - 1);
        }
    }
    
    static void heap_sort_range(std::vector<T>& arr, size_t low, size_t high) {
        size_t n = high - low + 1;
        
        for (int i = n / 2 - 1; i >= 0; --i) {
            heapify_range(arr, n, i, low);
        }
        
        for (int i = n - 1; i > 0; --i) {
            utils::Swap::swap(arr[low], arr[low + i]);
            heapify_range(arr, i, 0, low);
        }
    }
    
    static void heapify_range(std::vector<T>& arr, size_t n, size_t i, size_t offset) {
        size_t largest = i;
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;
        
        if (left < n && arr[offset + left] > arr[offset + largest]) {
            largest = left;
        }
        
        if (right < n && arr[offset + right] > arr[offset + largest]) {
            largest = right;
        }
        
        if (largest != i) {
            utils::Swap::swap(arr[offset + i], arr[offset + largest]);
            heapify_range(arr, n, largest, offset);
        }
    }
};

// Type aliases for common use cases
using IntSort = Sort<int>;
using DoubleSort = Sort<double>;
using StringSort = Sort<std::string>;

} // namespace algorithms
} // namespace dsa
