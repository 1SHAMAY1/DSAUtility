#pragma once
#include <iterator>

// Template-based binary search for random-access iterators
template<typename RandomIt, typename T>
bool binary_search(RandomIt first, RandomIt last, const T& value) {
    while (first < last) {
        RandomIt mid = first + (last - first) / 2;
        if (*mid == value) return true;
        else if (*mid < value) first = mid + 1;
        else last = mid;
    }
    return false;
} 