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
 * @brief Shell Sort Algorithm Implementation
 * Time Complexity: O(n log n) to O(n^2) depending on gap sequence
 * Space Complexity: O(1)
 * Stable: No
 */
template<typename T>
class ShellSort {
public:
    // Standard shell sort (using Shell's original gap sequence)
    static void sort(std::vector<T>& arr) {
        size_t n = arr.size();
        for (size_t gap = n / 2; gap > 0; gap /= 2) {
            for (size_t i = gap; i < n; ++i) {
                T temp = arr[i];
                size_t j = i;
                while (j >= gap && arr[j - gap] > temp) {
                    arr[j] = arr[j - gap];
                    j -= gap;
                }
                arr[j] = temp;
            }
        }
    }

    // Shell sort with custom gap sequence
    static void sort_with_gaps(std::vector<T>& arr, const std::vector<size_t>& gaps) {
        size_t n = arr.size();
        for (size_t gap : gaps) {
            for (size_t i = gap; i < n; ++i) {
                T temp = arr[i];
                size_t j = i;
                while (j >= gap && arr[j - gap] > temp) {
                    arr[j] = arr[j - gap];
                    j -= gap;
                }
                arr[j] = temp;
            }
        }
    }

    // Shell sort with Sedgewick's gap sequence
    static void sort_sedgewick(std::vector<T>& arr) {
        size_t n = arr.size();
        std::vector<size_t> gaps = sedgewick_gaps(n);
        sort_with_gaps(arr, gaps);
    }

    // Shell sort with Hibbard's gap sequence
    static void sort_hibbard(std::vector<T>& arr) {
        size_t n = arr.size();
        std::vector<size_t> gaps = hibbard_gaps(n);
        sort_with_gaps(arr, gaps);
    }

    // Shell sort with Pratt's gap sequence
    static void sort_pratt(std::vector<T>& arr) {
        size_t n = arr.size();
        std::vector<size_t> gaps = pratt_gaps(n);
        sort_with_gaps(arr, gaps);
    }

    // Shell sort with custom comparator
    template<typename Compare>
    static void sort(std::vector<T>& arr, Compare comp) {
        size_t n = arr.size();
        for (size_t gap = n / 2; gap > 0; gap /= 2) {
            for (size_t i = gap; i < n; ++i) {
                T temp = arr[i];
                size_t j = i;
                while (j >= gap && comp(temp, arr[j - gap])) {
                    arr[j] = arr[j - gap];
                    j -= gap;
                }
                arr[j] = temp;
            }
        }
    }

    // Utility functions
    static bool is_sorted(const std::vector<T>& arr) {
        return std::is_sorted(arr.begin(), arr.end());
    }

    static std::vector<size_t> shell_gaps(size_t n) {
        std::vector<size_t> gaps;
        for (size_t gap = n / 2; gap > 0; gap /= 2) {
            gaps.push_back(gap);
        }
        return gaps;
    }

    static std::vector<size_t> sedgewick_gaps(size_t n) {
        std::vector<size_t> gaps;
        size_t k = 0;
        size_t gap;
        do {
            if (k % 2 == 0) {
                gap = 9 * (1 << (2 * k)) - 9 * (1 << k) + 1;
            } else {
                gap = 8 * (1 << (2 * k)) - 6 * (1 << (k + 1)) + 1;
            }
            if (gap < n)
                gaps.insert(gaps.begin(), gap);
            ++k;
        } while (gap < n);
        return gaps;
    }

    static std::vector<size_t> hibbard_gaps(size_t n) {
        std::vector<size_t> gaps;
        size_t k = 1;
        size_t gap = 1;
        while (gap < n) {
            gaps.insert(gaps.begin(), gap);
            ++k;
            gap = (1 << k) - 1;
        }
        return gaps;
    }

    static std::vector<size_t> pratt_gaps(size_t n) {
        std::vector<size_t> gaps;
        for (size_t p = 1; p < n; p *= 2) {
            for (size_t q = 1; p * q < n; q *= 3) {
                gaps.push_back(p * q);
            }
        }
        std::sort(gaps.begin(), gaps.end(), std::greater<size_t>());
        return gaps;
    }
};

// Type aliases for common use cases
using IntShellSort = ShellSort<int>;
using DoubleShellSort = ShellSort<double>;
using StringShellSort = ShellSort<std::string>;

} // namespace algorithms
} // namespace dsa
