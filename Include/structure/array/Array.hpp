#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>

template<typename T>
class Array {
public:
    Array() : currentSize(0) {}

    void add(const T& value) {
        if (currentSize < data.size()) {
            data[currentSize] = value;
        } else {
            data.push_back(value);
        }
        ++currentSize;
    }

    void print() const {
        for (size_t i = 0; i < currentSize; ++i) {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }

    T& operator[](size_t index) {
        if (index >= currentSize) throw std::out_of_range("Index out of range");
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= currentSize) throw std::out_of_range("Index out of range");
        return data[index];
    }

    size_t size() const {
        return currentSize;
    }

    bool empty() const {
        return currentSize == 0;
    }

    void removeAt(size_t index) {
        if (index >= currentSize) return;
        for (size_t i = index; i < currentSize - 1; ++i) {
            data[i] = data[i + 1];
        }
        --currentSize;
    }

    T& back() {
        if (empty()) throw std::out_of_range("Array is empty");
        return data[currentSize - 1];
    }

    const T& back() const {
        if (empty()) throw std::out_of_range("Array is empty");
        return data[currentSize - 1];
    }

    void removeLast() {
        if (!empty()) --currentSize;
    }

private:
    std::vector<T> data;
    size_t currentSize;
};
