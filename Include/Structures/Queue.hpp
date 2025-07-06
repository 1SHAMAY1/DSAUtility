#pragma once
#include <deque>
#include <stdexcept>
#include <iostream>
#include <initializer_list>
#include "../Utils/Print.hpp"
#include "../Utils/Logger.hpp"

namespace dsa {
namespace structures {

/**
 * @brief Queue Data Structure with template support
 */
template<typename T>
class Queue {
private:
    std::deque<T> data;
    size_t capacity;
    static constexpr size_t DEFAULT_CAPACITY = 10;

public:
    // Constructors
    Queue() : capacity(DEFAULT_CAPACITY) {
        data.reserve(capacity);
    }

    explicit Queue(size_t initial_capacity) : capacity(initial_capacity) {
        data.reserve(capacity);
    }

    Queue(std::initializer_list<T> init_list) : capacity(std::max(init_list.size(), DEFAULT_CAPACITY)) {
        data.reserve(capacity);
        for (const auto& item : init_list) {
            push(item);
        }
    }

    Queue(const Queue& other) : data(other.data), capacity(other.capacity) {}

    Queue(Queue&& other) noexcept 
        : data(std::move(other.data)), capacity(other.capacity) {
        other.capacity = 0;
    }

    // Destructor
    ~Queue() = default;

    // Assignment operators
    Queue& operator=(const Queue& other) {
        if (this != &other) {
            data = other.data;
            capacity = other.capacity;
        }
        return *this;
    }

    Queue& operator=(Queue&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);
            capacity = other.capacity;
            other.capacity = 0;
        }
        return *this;
    }

    // Capacity
    bool empty() const { return data.empty(); }
    size_t size() const { return data.size(); }
    size_t max_size() const { return data.max_size(); }

    // Element access
    T& front() {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return data.front();
    }

    const T& front() const {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return data.front();
    }

    T& back() {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return data.back();
    }

    const T& back() const {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return data.back();
    }

    // Modifiers
    void push(const T& value) {
        if (data.size() >= capacity) {
            resize_capacity();
        }
        data.push_back(value);
    }

    void push(T&& value) {
        if (data.size() >= capacity) {
            resize_capacity();
        }
        data.push_back(std::move(value));
    }

    void pop() {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        data.pop_front();
    }

    template<typename... Args>
    void emplace(Args&&... args) {
        if (data.size() >= capacity) {
            resize_capacity();
        }
        data.emplace_back(std::forward<Args>(args)...);
    }

    void clear() {
        data.clear();
    }

    // Queue operations
    void swap(Queue& other) {
        std::swap(data, other.data);
        std::swap(capacity, other.capacity);
    }

    // Search operations
    bool contains(const T& value) const {
        return std::find(data.begin(), data.end(), value) != data.end();
    }

    size_t count(const T& value) const {
        return std::count(data.begin(), data.end(), value);
    }

    size_t find(const T& value) const {
        auto it = std::find(data.begin(), data.end(), value);
        return (it != data.end()) ? std::distance(data.begin(), it) : static_cast<size_t>(-1);
    }

    // Queue manipulation
    void reverse() {
        std::reverse(data.begin(), data.end());
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

    // Iterators
    typename std::deque<T>::iterator begin() { return data.begin(); }
    typename std::deque<T>::const_iterator begin() const { return data.begin(); }
    typename std::deque<T>::iterator end() { return data.end(); }
    typename std::deque<T>::const_iterator end() const { return data.end(); }

    // Reverse iterators
    typename std::deque<T>::reverse_iterator rbegin() { return data.rbegin(); }
    typename std::deque<T>::const_reverse_iterator rbegin() const { return data.rbegin(); }
    typename std::deque<T>::reverse_iterator rend() { return data.rend(); }
    typename std::deque<T>::const_reverse_iterator rend() const { return data.rend(); }

    // Comparison operators
    bool operator==(const Queue& other) const {
        return data == other.data;
    }

    bool operator!=(const Queue& other) const {
        return data != other.data;
    }

    bool operator<(const Queue& other) const {
        return data < other.data;
    }

    bool operator<=(const Queue& other) const {
        return data <= other.data;
    }

    bool operator>(const Queue& other) const {
        return data > other.data;
    }

    bool operator>=(const Queue& other) const {
        return data >= other.data;
    }

    // Utility functions
    void print() const {
        std::cout << "Queue (front to back): ";
        for (size_t i = 0; i < data.size(); ++i) {
            std::cout << data[i];
            if (i < data.size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << std::endl;
    }

    void print_as_array() const {
        utils::Print::printVector(std::vector<T>(data.begin(), data.end()), "Queue: ");
        std::cout << std::endl;
    }

    std::string to_string() const {
        std::stringstream ss;
        ss << "Queue[size=" << size() << ", capacity=" << capacity << "]";
        return ss.str();
    }

    // Queue-specific operations
    bool is_full() const {
        return size() >= capacity;
    }

    size_t get_capacity() const {
        return capacity;
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > capacity) {
            capacity = new_capacity;
            data.reserve(capacity);
        }
    }

    // Advanced queue operations
    void move_to_back() {
        if (size() < 2) {
            return; // Nothing to move
        }
        T front_element = front();
        pop();
        push(front_element);
    }

    void move_to_front() {
        if (size() < 2) {
            return; // Nothing to move
        }
        T back_element = back();
        data.pop_back();
        data.push_front(back_element);
    }

    void rotate(size_t positions) {
        if (empty() || positions == 0) {
            return;
        }
        positions %= size();
        for (size_t i = 0; i < positions; ++i) {
            move_to_back();
        }
    }

    void rotate_reverse(size_t positions) {
        if (empty() || positions == 0) {
            return;
        }
        positions %= size();
        for (size_t i = 0; i < positions; ++i) {
            move_to_front();
        }
    }

    // Queue validation
    bool is_valid() const {
        return size() <= capacity;
    }

    // Memory management
    void shrink_to_fit() {
        data.shrink_to_fit();
        capacity = data.capacity();
    }

    // Queue statistics
    T get_front_element() const {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return data.front();
    }

    T get_back_element() const {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return data.back();
    }

    // Queue operations with error handling
    bool try_push(const T& value) {
        try {
            push(value);
            return true;
        } catch (...) {
            return false;
        }
    }

    bool try_pop() {
        try {
            pop();
            return true;
        } catch (...) {
            return false;
        }
    }

    std::pair<bool, T> try_front() const {
        if (empty()) {
            return {false, T{}};
        }
        return {true, data.front()};
    }

    std::pair<bool, T> try_back() const {
        if (empty()) {
            return {false, T{}};
        }
        return {true, data.back()};
    }

private:
    void resize_capacity() {
        capacity = static_cast<size_t>(capacity * 1.5);
        data.reserve(capacity);
    }
};

// Type aliases for common use cases
using IntQueue = Queue<int>;
using DoubleQueue = Queue<double>;
using StringQueue = Queue<std::string>;
using CharQueue = Queue<char>;

// Utility functions for queue operations
template<typename T>
void reverse_queue(Queue<T>& queue) {
    Queue<T> temp;
    std::vector<T> elements;
    
    // Extract all elements
    while (!queue.empty()) {
        elements.push_back(queue.front());
        queue.pop();
    }
    
    // Push back in reverse order
    for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
        queue.push(*it);
    }
}

template<typename T>
Queue<T> copy_queue(const Queue<T>& queue) {
    return Queue<T>(queue);
}

template<typename T>
bool are_queues_equal(const Queue<T>& queue1, const Queue<T>& queue2) {
    if (queue1.size() != queue2.size()) {
        return false;
    }
    
    Queue<T> temp1 = copy_queue(queue1);
    Queue<T> temp2 = copy_queue(queue2);
    
    while (!temp1.empty() && !temp2.empty()) {
        if (temp1.front() != temp2.front()) {
            return false;
        }
        temp1.pop();
        temp2.pop();
    }
    
    return temp1.empty() && temp2.empty();
}

// Priority queue-like operations
template<typename T>
void sort_queue(Queue<T>& queue) {
    std::vector<T> elements;
    while (!queue.empty()) {
        elements.push_back(queue.front());
        queue.pop();
    }
    
    std::sort(elements.begin(), elements.end());
    
    for (const auto& element : elements) {
        queue.push(element);
    }
}

template<typename T, typename Compare>
void sort_queue(Queue<T>& queue, Compare comp) {
    std::vector<T> elements;
    while (!queue.empty()) {
        elements.push_back(queue.front());
        queue.pop();
    }
    
    std::sort(elements.begin(), elements.end(), comp);
    
    for (const auto& element : elements) {
        queue.push(element);
    }
}

} // namespace structures
} // namespace dsa
