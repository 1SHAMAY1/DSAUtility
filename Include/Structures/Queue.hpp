#pragma once
#include <deque>
#include <stdexcept>
#include <iostream>
#include <initializer_list>
#include <vector>
#include <algorithm>
#include "../Utils/Print.hpp"
#include "../Utils/Logger.hpp"

namespace dsa {
namespace structures {

/**
 * @brief Base Queue Data Structure (FIFO)
 */
template<typename T>
class Queue {
protected:
    std::deque<T> data;
    size_t capacity_;

public:
    // Constructors
    Queue() : capacity_(std::numeric_limits<size_t>::max()) {}
    
    explicit Queue(size_t capacity) : capacity_(capacity) {}
    
    Queue(std::initializer_list<T> init_list) : capacity_(std::numeric_limits<size_t>::max()) {
        for (const auto& item : init_list) {
            push(item);
        }
    }
    
    Queue(const Queue& other) : data(other.data), capacity_(other.capacity_) {}
    
    Queue(Queue&& other) noexcept 
        : data(std::move(other.data)), capacity_(other.capacity_) {}
    
    // Destructor
    virtual ~Queue() = default;
    
    // Assignment operators
    Queue& operator=(const Queue& other) {
        if (this != &other) {
            data = other.data;
            capacity_ = other.capacity_;
        }
        return *this;
    }
    
    Queue& operator=(Queue&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);
            capacity_ = other.capacity_;
        }
        return *this;
    }
    
    // Capacity
    bool empty() const { return data.empty(); }
    size_t size() const { return data.size(); }
    size_t capacity() const { return capacity_; }
    bool full() const { return size() >= capacity_; }
    
    // Element access
    virtual T& front() {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return data.front();
    }
    
    virtual const T& front() const {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return data.front();
    }
    
    virtual T& back() {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return data.back();
    }
    
    virtual const T& back() const {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return data.back();
    }
    
    // Modifiers
    virtual void push(const T& value) {
        if (full()) {
            throw std::overflow_error("Queue is full");
        }
        data.push_back(value);
    }
    
    virtual void push(T&& value) {
        if (full()) {
            throw std::overflow_error("Queue is full");
        }
        data.push_back(std::move(value));
    }
    
    virtual void pop() {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        data.pop_front();
    }
    
    virtual T pop_and_get() {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        T value = std::move(data.front());
        data.pop_front();
        return value;
    }
    
    template<typename... Args>
    void emplace(Args&&... args) {
        if (full()) {
            throw std::overflow_error("Queue is full");
        }
        data.emplace_back(std::forward<Args>(args)...);
    }
    
    virtual void clear() {
        data.clear();
    }
    
    virtual void resize(size_t new_capacity) {
        if (new_capacity < size()) {
            throw std::invalid_argument("New capacity is less than current size");
        }
        capacity_ = new_capacity;
    }
    
    // Queue operations
    virtual void swap(Queue& other) {
        std::swap(data, other.data);
        std::swap(capacity_, other.capacity_);
    }
    
    virtual Queue<T> get_reverse() const {
        Queue<T> reversed(capacity_);
        std::vector<T> temp(data.begin(), data.end());
        std::reverse(temp.begin(), temp.end());
        for (const auto& item : temp) {
            reversed.push(item);
        }
        return reversed;
    }
    
    virtual void reverse() {
        std::reverse(data.begin(), data.end());
    }
    
    // Search operations
    virtual bool contains(const T& value) const {
        return std::find(data.begin(), data.end(), value) != data.end();
    }
    
    virtual size_t count(const T& value) const {
        return std::count(data.begin(), data.end(), value);
    }
    
    virtual size_t find(const T& value) const {
        auto it = std::find(data.begin(), data.end(), value);
        return (it != data.end()) ? std::distance(data.begin(), it) : static_cast<size_t>(-1);
    }
    
    // Mathematical operations (for numeric types)
    template<typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, T>::type sum() const {
        T result = T{};
        for (const auto& item : data) {
            result += item;
        }
        return result;
    }
    
    template<typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, T>::type min() const {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return *std::min_element(data.begin(), data.end());
    }
    
    template<typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, T>::type max() const {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return *std::max_element(data.begin(), data.end());
    }
    
    template<typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, double>::type average() const {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return static_cast<double>(sum()) / size();
    }
    
    // Comparison operators
    virtual bool operator==(const Queue& other) const {
        return data == other.data;
    }
    
    virtual bool operator!=(const Queue& other) const {
        return data != other.data;
    }
    
    virtual bool operator<(const Queue& other) const {
        return data < other.data;
    }
    
    virtual bool operator<=(const Queue& other) const {
        return data <= other.data;
    }
    
    virtual bool operator>(const Queue& other) const {
        return data > other.data;
    }
    
    virtual bool operator>=(const Queue& other) const {
        return data >= other.data;
    }
    
    // Utility functions
    virtual void print() const {
        utils::Print::printVectorWithPrefix(std::vector<T>(data.begin(), data.end()), "Queue: ");
        std::cout << std::endl;
    }
    
    virtual void print_reverse() const {
        std::vector<T> reversed(data.rbegin(), data.rend());
        utils::Print::printVectorWithPrefix(reversed, "Queue (reverse): ");
        std::cout << std::endl;
    }
    
    virtual std::string to_string() const {
        std::stringstream ss;
        ss << "Queue[size=" << size() << ", capacity=" << capacity_ << "]";
        return ss.str();
    }
    
    // Iterator support
    typename std::deque<T>::iterator begin() { return data.begin(); }
    typename std::deque<T>::const_iterator begin() const { return data.begin(); }
    typename std::deque<T>::iterator end() { return data.end(); }
    typename std::deque<T>::const_iterator end() const { return data.end(); }
    
    typename std::deque<T>::reverse_iterator rbegin() { return data.rbegin(); }
    typename std::deque<T>::const_reverse_iterator rbegin() const { return data.rbegin(); }
    typename std::deque<T>::reverse_iterator rend() { return data.rend(); }
    typename std::deque<T>::const_reverse_iterator rend() const { return data.rend(); }
};

// Type aliases for common use cases
using IntQueue = Queue<int>;
using DoubleQueue = Queue<double>;
using StringQueue = Queue<std::string>;

} // namespace structures
} // namespace dsa
