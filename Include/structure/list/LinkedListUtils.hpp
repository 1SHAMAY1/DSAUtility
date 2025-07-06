#pragma once
#include "LinkedList.hpp"

template <typename T>
T getFirst(const LinkedList<T>& list) {
    if (list.empty()) throw std::runtime_error("List is empty");
    return list.front(); // only if front() is implemented
}
