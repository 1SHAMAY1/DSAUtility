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
 * @brief Radix Sort Algorithm Implementation
 * Time Complexity: O(d * (n + k)) where d is number of digits, k is base
 * Space Complexity: O(n + k)
 * Stable: Yes
 */
template<typename T>
class RadixSort {
public:
    // Standard radix sort (base 10)
    static void sort(std::vector<T>& arr) {
        if (arr.empty()) return;
        
        T max_val = *std::max_element(arr.begin(), arr.end());
        
        for (T exp = 1; max_val / exp > 0; exp *= 10) {
            counting_sort_by_digit(arr, exp);
        }
    }
    
    // Radix sort with custom base
    static void sort_with_base(std::vector<T>& arr, int base) {
        if (arr.empty()) return;
        
        T max_val = *std::max_element(arr.begin(), arr.end());
        
        for (T exp = 1; max_val / exp > 0; exp *= base) {
            counting_sort_by_digit_with_base(arr, exp, base);
        }
    }
    
    // LSD (Least Significant Digit) radix sort
    static void lsd_sort(std::vector<T>& arr) {
        if (arr.empty()) return;
        
        T max_val = *std::max_element(arr.begin(), arr.end());
        int max_digits = static_cast<int>(std::log10(max_val)) + 1;
        
        for (int digit = 0; digit < max_digits; ++digit) {
            counting_sort_by_digit_position(arr, digit);
        }
    }
    
    // MSD (Most Significant Digit) radix sort
    static void msd_sort(std::vector<T>& arr) {
        if (arr.empty()) return;
        
        T max_val = *std::max_element(arr.begin(), arr.end());
        int max_digits = static_cast<int>(std::log10(max_val)) + 1;
        
        msd_sort_recursive(arr, 0, arr.size() - 1, max_digits - 1);
    }
    
    // Radix sort with custom digit extraction
    template<typename DigitFunc>
    static void sort_with_digit_func(std::vector<T>& arr, DigitFunc digit_func) {
        if (arr.empty()) return;
        
        T max_val = *std::max_element(arr.begin(), arr.end());
        int max_digits = static_cast<int>(std::log10(max_val)) + 1;
        
        for (int digit = 0; digit < max_digits; ++digit) {
            counting_sort_by_custom_digit(arr, digit, digit_func);
        }
    }
    
    // Radix sort for strings
    static void sort_strings(std::vector<std::string>& arr) {
        if (arr.empty()) return;
        
        // Find maximum length
        size_t max_length = 0;
        for (const auto& str : arr) {
            max_length = std::max(max_length, str.length());
        }
        
        // Sort from least significant character to most significant
        for (int pos = max_length - 1; pos >= 0; --pos) {
            counting_sort_by_char_position(arr, pos);
        }
    }
    
    // Radix sort for floating point numbers
    static void sort_floats(std::vector<double>& arr) {
        if (arr.empty()) return;
        
        // Convert to integers by multiplying by a large factor
        const double factor = 1000000.0;
        std::vector<int64_t> int_arr(arr.size());
        
        for (size_t i = 0; i < arr.size(); ++i) {
            int_arr[i] = static_cast<int64_t>(arr[i] * factor);
        }
        
        // Sort integers
        sort(int_arr);
        
        // Convert back to doubles
        for (size_t i = 0; i < arr.size(); ++i) {
            arr[i] = static_cast<double>(int_arr[i]) / factor;
        }
    }
    
    // Radix sort with negative number support
    static void sort_with_negatives(std::vector<T>& arr) {
        if (arr.empty()) return;
        
        // Separate positive and negative numbers
        std::vector<T> positives, negatives;
        
        for (const auto& val : arr) {
            if (val >= 0) {
                positives.push_back(val);
            } else {
                negatives.push_back(-val); // Make positive for sorting
            }
        }
        
        // Sort positive numbers
        if (!positives.empty()) {
            sort(positives);
        }
        
        // Sort negative numbers (as positive, then reverse)
        if (!negatives.empty()) {
            sort(negatives);
            std::reverse(negatives.begin(), negatives.end());
            for (auto& val : negatives) {
                val = -val; // Convert back to negative
            }
        }
        
        // Combine results
        arr.clear();
        arr.insert(arr.end(), negatives.begin(), negatives.end());
        arr.insert(arr.end(), positives.begin(), positives.end());
    }
    
    // Utility functions
    static bool is_sorted(const std::vector<T>& arr) {
        return std::is_sorted(arr.begin(), arr.end());
    }
    
    static int get_max_digits(const std::vector<T>& arr) {
        if (arr.empty()) return 0;
        
        T max_val = *std::max_element(arr.begin(), arr.end());
        return static_cast<int>(std::log10(max_val)) + 1;
    }
    
    static int get_digit_count(T num) {
        if (num == 0) return 1;
        return static_cast<int>(std::log10(std::abs(num))) + 1;
    }
    
    // Get digit at specific position
    static int get_digit(T num, int position) {
        return static_cast<int>((num / static_cast<T>(std::pow(10, position))) % 10);
    }
    
    // Get digit with custom base
    static int get_digit_with_base(T num, int position, int base) {
        return static_cast<int>((num / static_cast<T>(std::pow(base, position))) % base);
    }

private:
    // Standard counting sort by digit
    static void counting_sort_by_digit(std::vector<T>& arr, T exp) {
        std::vector<T> output(arr.size());
        std::vector<int> count(10, 0);
        
        // Count occurrences
        for (const auto& val : arr) {
            count[(val / exp) % 10]++;
        }
        
        // Calculate positions
        for (int i = 1; i < 10; ++i) {
            count[i] += count[i - 1];
        }
        
        // Build output array
        for (int i = arr.size() - 1; i >= 0; --i) {
            output[count[(arr[i] / exp) % 10] - 1] = arr[i];
            count[(arr[i] / exp) % 10]--;
        }
        
        arr = output;
    }
    
    // Counting sort by digit with custom base
    static void counting_sort_by_digit_with_base(std::vector<T>& arr, T exp, int base) {
        std::vector<T> output(arr.size());
        std::vector<int> count(base, 0);
        
        // Count occurrences
        for (const auto& val : arr) {
            count[(val / exp) % base]++;
        }
        
        // Calculate positions
        for (int i = 1; i < base; ++i) {
            count[i] += count[i - 1];
        }
        
        // Build output array
        for (int i = arr.size() - 1; i >= 0; --i) {
            output[count[(arr[i] / exp) % base] - 1] = arr[i];
            count[(arr[i] / exp) % base]--;
        }
        
        arr = output;
    }
    
    // Counting sort by digit position
    static void counting_sort_by_digit_position(std::vector<T>& arr, int digit_pos) {
        std::vector<T> output(arr.size());
        std::vector<int> count(10, 0);
        
        // Count occurrences
        for (const auto& val : arr) {
            count[get_digit(val, digit_pos)]++;
        }
        
        // Calculate positions
        for (int i = 1; i < 10; ++i) {
            count[i] += count[i - 1];
        }
        
        // Build output array
        for (int i = arr.size() - 1; i >= 0; --i) {
            output[count[get_digit(arr[i], digit_pos)] - 1] = arr[i];
            count[get_digit(arr[i], digit_pos)]--;
        }
        
        arr = output;
    }
    
    // Counting sort by custom digit function
    template<typename DigitFunc>
    static void counting_sort_by_custom_digit(std::vector<T>& arr, int digit, DigitFunc digit_func) {
        std::vector<T> output(arr.size());
        std::vector<int> count(10, 0);
        
        // Count occurrences
        for (const auto& val : arr) {
            count[digit_func(val, digit)]++;
        }
        
        // Calculate positions
        for (int i = 1; i < 10; ++i) {
            count[i] += count[i - 1];
        }
        
        // Build output array
        for (int i = arr.size() - 1; i >= 0; --i) {
            output[count[digit_func(arr[i], digit)] - 1] = arr[i];
            count[digit_func(arr[i], digit)]--;
        }
        
        arr = output;
    }
    
    // Counting sort by character position for strings
    static void counting_sort_by_char_position(std::vector<std::string>& arr, int pos) {
        std::vector<std::string> output(arr.size());
        std::vector<int> count(256, 0); // ASCII characters
        
        // Count occurrences
        for (const auto& str : arr) {
            char c = (pos < static_cast<int>(str.length())) ? str[pos] : '\0';
            count[static_cast<unsigned char>(c)]++;
        }
        
        // Calculate positions
        for (int i = 1; i < 256; ++i) {
            count[i] += count[i - 1];
        }
        
        // Build output array
        for (int i = arr.size() - 1; i >= 0; --i) {
            char c = (pos < static_cast<int>(arr[i].length())) ? arr[i][pos] : '\0';
            output[count[static_cast<unsigned char>(c)] - 1] = arr[i];
            count[static_cast<unsigned char>(c)]--;
        }
        
        arr = output;
    }
    
    // MSD radix sort recursive implementation
    static void msd_sort_recursive(std::vector<T>& arr, size_t left, size_t right, int digit) {
        if (left >= right || digit < 0) return;
        
        // Use counting sort for current digit
        std::vector<T> output(right - left + 1);
        std::vector<int> count(10, 0);
        
        // Count occurrences
        for (size_t i = left; i <= right; ++i) {
            count[get_digit(arr[i], digit)]++;
        }
        
        // Calculate positions
        for (int i = 1; i < 10; ++i) {
            count[i] += count[i - 1];
        }
        
        // Build output array
        for (int i = right - left; i >= 0; --i) {
            output[count[get_digit(arr[left + i], digit)] - 1] = arr[left + i];
            count[get_digit(arr[left + i], digit)]--;
        }
        
        // Copy back to original array
        for (size_t i = 0; i <= right - left; ++i) {
            arr[left + i] = output[i];
        }
        
        // Recursively sort each bucket
        size_t start = left;
        for (int d = 0; d < 10; ++d) {
            size_t end = start;
            while (end <= right && get_digit(arr[end], digit) == d) {
                ++end;
            }
            
            if (end > start + 1) {
                msd_sort_recursive(arr, start, end - 1, digit - 1);
            }
            start = end;
        }
    }
};

// Type aliases for common use cases
using IntRadixSort = RadixSort<int>;
using LongRadixSort = RadixSort<long>;
using UIntRadixSort = RadixSort<unsigned int>;

} // namespace algorithms
} // namespace dsa
