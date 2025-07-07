#pragma once
#include <iterator>
#include <utility>

namespace algo {
    // Generic sort interface (for future extension)
    template<typename RandomIt>
    void sort(RandomIt first, RandomIt last);

    // Utility swap (if needed)
    template<typename T>
    void swap(T& a, T& b) {
        T temp = std::move(a);
        a = std::move(b);
        b = std::move(temp);
    }
} 