#pragma once
#include <iterator>
#include <algorithm>

namespace algo {
    template<typename RandomIt>
    void heapSort(RandomIt first, RandomIt last) {
        std::make_heap(first, last);
        std::sort_heap(first, last);
    }
} 