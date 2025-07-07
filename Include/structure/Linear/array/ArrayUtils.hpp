#pragma once

#include <iostream>
#include <algorithm>

template<typename T>
void printArray(const T* arr, size_t size) {
    for (size_t i = 0; i < size; ++i)
        std::cout << arr[i] << " ";
    std::cout << std::endl;
}

template<typename T>
void reverseArray(T* arr, size_t size) {
    for (size_t i = 0; i < size / 2; ++i)
        std::swap(arr[i], arr[size - i - 1]);
}
