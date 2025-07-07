#pragma once
#include <iterator>
#include <utility>

namespace algo {
    template<typename RandomIt>
    void quickSort(RandomIt first, RandomIt last) {
        if (first < last) {
            auto pivot = *first;
            RandomIt left = first + 1;
            RandomIt right = last - 1;
            while (left <= right) {
                while (left <= right && *left < pivot) ++left;
                while (left <= right && *right > pivot) --right;
                if (left < right) {
                    std::iter_swap(left, right);
                    ++left; --right;
                }
            }
            std::iter_swap(first, right);
            quickSort(first, right);
            quickSort(right + 1, last);
        }
    }
} 