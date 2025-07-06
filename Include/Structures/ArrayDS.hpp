#pragma once
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <initializer_list>
#include <type_traits>
#include "../Utils/Print.hpp"
#include "../Utils/Logger.hpp"

namespace dsa {
namespace structures {

/**
 * @brief Dynamic Array Data Structure with template support
 */
template<typename T>
class ArrayDS {
private:
    std::vector<T> data;
    size_t capacity;
    size_t size_;
    static constexpr size_t DEFAULT_CAPACITY = 10;
    static constexpr double GROWTH_FACTOR = 1.5;

public:
    // Constructors
    ArrayDS() : capacity(DEFAULT_CAPACITY), size_(0) {
        data.reserve(capacity);
    }

    explicit ArrayDS(size_t initial_capacity) : capacity(initial_capacity), size_(0) {
        data.reserve(capacity);
    }

    ArrayDS(std::initializer_list<T> init_list) : size_(init_list.size()) {
        capacity = std::max(size_, DEFAULT_CAPACITY);
        data.reserve(capacity);
        data.assign(init_list.begin(), init_list.end());
    }

    ArrayDS(const ArrayDS& other) : data(other.data), capacity(other.capacity), size_(other.size_) {}

    ArrayDS(ArrayDS&& other) noexcept 
        : data(std::move(other.data)), capacity(other.capacity), size_(other.size_) {
        other.capacity = 0;
        other.size_ = 0;
    }

    // Destructor
    ~ArrayDS() = default;

    // Assignment operators
    ArrayDS& operator=(const ArrayDS& other) {
        if (this != &other) {
            data = other.data;
            capacity = other.capacity;
            size_ = other.size_;
        }
        return *this;
    }

    ArrayDS& operator=(ArrayDS&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);
            capacity = other.capacity;
            size_ = other.size_;
            other.capacity = 0;
            other.size_ = 0;
        }
        return *this;
    }

    // Element access
    T& operator[](size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Array index out of bounds");
        }
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Array index out of bounds");
        }
        return data[index];
    }

    T& at(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Array index out of bounds");
        }
        return data[index];
    }

    const T& at(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Array index out of bounds");
        }
        return data[index];
    }

    T& front() {
        if (empty()) {
            throw std::out_of_range("Array is empty");
        }
        return data[0];
    }

    const T& front() const {
        if (empty()) {
            throw std::out_of_range("Array is empty");
        }
        return data[0];
    }

    T& back() {
        if (empty()) {
            throw std::out_of_range("Array is empty");
        }
        return data[size_ - 1];
    }

    const T& back() const {
        if (empty()) {
            throw std::out_of_range("Array is empty");
        }
        return data[size_ - 1];
    }

    // Capacity
    bool empty() const { return size_ == 0; }
    size_t size() const { return size_; }
    size_t max_size() const { return data.max_size(); }
    void reserve(size_t new_capacity) {
        if (new_capacity > capacity) {
            capacity = new_capacity;
            data.reserve(capacity);
        }
    }
    size_t get_capacity() const { return capacity; }

    // Modifiers
    void clear() {
        data.clear();
        size_ = 0;
    }

    void push_back(const T& value) {
        if (size_ >= capacity) {
            resize_capacity();
        }
        data.push_back(value);
        size_++;
    }

    void push_back(T&& value) {
        if (size_ >= capacity) {
            resize_capacity();
        }
        data.push_back(std::move(value));
        size_++;
    }

    void pop_back() {
        if (!empty()) {
            data.pop_back();
            size_--;
        }
    }

    void insert(size_t index, const T& value) {
        if (index > size_) {
            throw std::out_of_range("Insert index out of bounds");
        }
        if (size_ >= capacity) {
            resize_capacity();
        }
        data.insert(data.begin() + index, value);
        size_++;
    }

    void insert(size_t index, T&& value) {
        if (index > size_) {
            throw std::out_of_range("Insert index out of bounds");
        }
        if (size_ >= capacity) {
            resize_capacity();
        }
        data.insert(data.begin() + index, std::move(value));
        size_++;
    }

    void erase(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Erase index out of bounds");
        }
        data.erase(data.begin() + index);
        size_--;
    }

    void erase(size_t first, size_t last) {
        if (first >= size_ || last > size_ || first >= last) {
            throw std::out_of_range("Invalid erase range");
        }
        data.erase(data.begin() + first, data.begin() + last);
        size_ -= (last - first);
    }

    void resize(size_t new_size) {
        if (new_size > capacity) {
            capacity = new_size;
            data.reserve(capacity);
        }
        data.resize(new_size);
        size_ = new_size;
    }

    void resize(size_t new_size, const T& value) {
        if (new_size > capacity) {
            capacity = new_size;
            data.reserve(capacity);
        }
        data.resize(new_size, value);
        size_ = new_size;
    }

    // Search operations
    size_t find(const T& value) const {
        auto it = std::find(data.begin(), data.begin() + size_, value);
        return (it != data.begin() + size_) ? std::distance(data.begin(), it) : -1;
    }

    bool contains(const T& value) const {
        return find(value) != static_cast<size_t>(-1);
    }

    size_t count(const T& value) const {
        return std::count(data.begin(), data.begin() + size_, value);
    }

    // Sorting and manipulation
    void sort() {
        std::sort(data.begin(), data.begin() + size_);
    }

    template<typename Compare>
    void sort(Compare comp) {
        std::sort(data.begin(), data.begin() + size_, comp);
    }

    void reverse() {
        std::reverse(data.begin(), data.begin() + size_);
    }

    void unique() {
        auto it = std::unique(data.begin(), data.begin() + size_);
        size_ = std::distance(data.begin(), it);
        data.resize(size_);
    }

    // Array operations
    void fill(const T& value) {
        std::fill(data.begin(), data.begin() + size_, value);
    }

    void fill(size_t first, size_t last, const T& value) {
        if (first >= size_ || last > size_ || first >= last) {
            throw std::out_of_range("Invalid fill range");
        }
        std::fill(data.begin() + first, data.begin() + last, value);
    }

    // Mathematical operations (for numeric types)
    template<typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, T>::type sum() const {
        T result = T{};
        for (size_t i = 0; i < size_; ++i) {
            result += data[i];
        }
        return result;
    }

    template<typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, T>::type min() const {
        if (empty()) {
            throw std::out_of_range("Array is empty");
        }
        return *std::min_element(data.begin(), data.begin() + size_);
    }

    template<typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, T>::type max() const {
        if (empty()) {
            throw std::out_of_range("Array is empty");
        }
        return *std::max_element(data.begin(), data.begin() + size_);
    }

    template<typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, double>::type average() const {
        if (empty()) {
            throw std::out_of_range("Array is empty");
        }
        return static_cast<double>(sum()) / size_;
    }

    // Iterators
    typename std::vector<T>::iterator begin() { return data.begin(); }
    typename std::vector<T>::const_iterator begin() const { return data.begin(); }
    typename std::vector<T>::iterator end() { return data.begin() + size_; }
    typename std::vector<T>::const_iterator end() const { return data.begin() + size_; }

    // Comparison operators
    bool operator==(const ArrayDS& other) const {
        if (size_ != other.size_) return false;
        return std::equal(data.begin(), data.begin() + size_, other.data.begin());
    }

    bool operator!=(const ArrayDS& other) const {
        return !(*this == other);
    }

    bool operator<(const ArrayDS& other) const {
        return std::lexicographical_compare(data.begin(), data.begin() + size_,
                                          other.data.begin(), other.data.begin() + other.size_);
    }

    bool operator<=(const ArrayDS& other) const {
        return (*this < other) || (*this == other);
    }

    bool operator>(const ArrayDS& other) const {
        return other < *this;
    }

    bool operator>=(const ArrayDS& other) const {
        return !(*this < other);
    }

    // Utility functions
    void print() const {
        utils::Print::printVectorWithPrefix(data, "Array: ");
        std::cout << std::endl;
    }
    
    // Convert to std::vector
    std::vector<T> to_vector() const {
        return std::vector<T>(data.begin(), data.begin() + size_);
    }

    void print_range(size_t start, size_t end) const {
        if (start >= size_ || end > size_ || start >= end) {
            throw std::out_of_range("Invalid print range");
        }
        utils::Print::printVectorWithPrefix(std::vector<T>(data.begin() + start, data.begin() + end), "Range: ");
        std::cout << std::endl;
    }

    std::string to_string() const {
        std::stringstream ss;
        ss << "ArrayDS[size=" << size_ << ", capacity=" << capacity << "]";
        return ss.str();
    }

private:
    void resize_capacity() {
        capacity = static_cast<size_t>(capacity * GROWTH_FACTOR);
        data.reserve(capacity);
    }
};

// Specialization for common types
template<typename T>
using Array = ArrayDS<T>;

// Type aliases for common use cases
using IntArray = ArrayDS<int>;
using DoubleArray = ArrayDS<double>;
using StringArray = ArrayDS<std::string>;

} // namespace structures
} // namespace dsa
