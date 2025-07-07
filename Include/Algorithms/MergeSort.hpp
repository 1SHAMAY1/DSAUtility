#pragma once
#include <vector>
#include <iterator>
#include <algorithm>

namespace algo {
    template<typename RandomIt>
    void mergeSort(RandomIt first, RandomIt last) {
        auto dist = std::distance(first, last);
        if (dist > 1) {
            RandomIt mid = first + dist / 2;
            mergeSort(first, mid);
            mergeSort(mid, last);
            std::vector<typename std::iterator_traits<RandomIt>::value_type> temp;
            std::merge(first, mid, mid, last, std::back_inserter(temp));
            std::move(temp.begin(), temp.end(), first);
        }
    }
} 