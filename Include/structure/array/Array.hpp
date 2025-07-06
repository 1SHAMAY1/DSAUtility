#pragma once

#include <iostream>
#include <vector>

template<typename T>
class Array {
public:
    Array() = default;

    void add(const T& value) {
        data.push_back(value);
    }

    void print() const {
        for (const auto& val : data)
            std::cout << val << " ";
        std::cout << std::endl;
    }

    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    size_t size() const {
        return data.size();
    }

private:
    std::vector<T> data;
};
