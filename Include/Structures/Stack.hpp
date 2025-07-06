#pragma once
#include <vector>
#include <stdexcept>
#include <iostream>
#include <initializer_list>
#include "../Utils/Print.hpp"
#include "../Utils/Logger.hpp"

namespace dsa {
namespace structures {

/**
 * @brief Stack Data Structure with template support
 */
template<typename T>
class Stack {
private:
    std::vector<T> data;
    size_t capacity;
    static constexpr size_t DEFAULT_CAPACITY = 10;

public:
    // Constructors
    Stack() : capacity(DEFAULT_CAPACITY) {
        data.reserve(capacity);
    }

    explicit Stack(size_t initial_capacity) : capacity(initial_capacity) {
        data.reserve(capacity);
    }

    Stack(std::initializer_list<T> init_list) : capacity(std::max(init_list.size(), DEFAULT_CAPACITY)) {
        data.reserve(capacity);
        for (auto it = init_list.end() - 1; it >= init_list.begin(); --it) {
            push(*it);
        }
    }

    Stack(const Stack& other) : data(other.data), capacity(other.capacity) {}

    Stack(Stack&& other) noexcept 
        : data(std::move(other.data)), capacity(other.capacity) {
        other.capacity = 0;
    }

    // Destructor
    ~Stack() = default;

    // Assignment operators
    Stack& operator=(const Stack& other) {
        if (this != &other) {
            data = other.data;
            capacity = other.capacity;
        }
        return *this;
    }

    Stack& operator=(Stack&& other) noexcept {
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
    T& top() {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return data.back();
    }

    const T& top() const {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
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
            throw std::out_of_range("Stack is empty");
        }
        data.pop_back();
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

    // Stack operations
    void swap(Stack& other) {
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

    // Stack manipulation
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
            throw std::out_of_range("Stack is empty");
        }
        return *std::min_element(data.begin(), data.end());
    }

    template<typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, T>::type max() const {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return *std::max_element(data.begin(), data.end());
    }

    template<typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, double>::type average() const {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return static_cast<double>(sum()) / size();
    }

    // Iterators
    typename std::vector<T>::iterator begin() { return data.begin(); }
    typename std::vector<T>::const_iterator begin() const { return data.begin(); }
    typename std::vector<T>::iterator end() { return data.end(); }
    typename std::vector<T>::const_iterator end() const { return data.end(); }

    // Reverse iterators
    typename std::vector<T>::reverse_iterator rbegin() { return data.rbegin(); }
    typename std::vector<T>::const_reverse_iterator rbegin() const { return data.rbegin(); }
    typename std::vector<T>::reverse_iterator rend() { return data.rend(); }
    typename std::vector<T>::const_reverse_iterator rend() const { return data.rend(); }

    // Comparison operators
    bool operator==(const Stack& other) const {
        return data == other.data;
    }

    bool operator!=(const Stack& other) const {
        return data != other.data;
    }

    bool operator<(const Stack& other) const {
        return data < other.data;
    }

    bool operator<=(const Stack& other) const {
        return data <= other.data;
    }

    bool operator>(const Stack& other) const {
        return data > other.data;
    }

    bool operator>=(const Stack& other) const {
        return data >= other.data;
    }

    // Utility functions
    void print() const {
        std::cout << "Stack (top to bottom): ";
        for (auto it = data.rbegin(); it != data.rend(); ++it) {
            std::cout << *it;
            if (std::next(it) != data.rend()) {
                std::cout << " | ";
            }
        }
        std::cout << std::endl;
    }

    void print_as_array() const {
        utils::Print::printVector(data, "Stack: ");
        std::cout << std::endl;
    }

    std::string to_string() const {
        std::stringstream ss;
        ss << "Stack[size=" << size() << ", capacity=" << capacity << "]";
        return ss.str();
    }

    // Stack-specific operations
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

    // Advanced stack operations
    void duplicate_top() {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        push(top());
    }

    void swap_top_two() {
        if (size() < 2) {
            throw std::out_of_range("Stack has fewer than 2 elements");
        }
        std::swap(data[data.size() - 1], data[data.size() - 2]);
    }

    void rotate_top_three() {
        if (size() < 3) {
            throw std::out_of_range("Stack has fewer than 3 elements");
        }
        // Rotate: top -> second, second -> third, third -> top
        T temp = data[data.size() - 1];
        data[data.size() - 1] = data[data.size() - 2];
        data[data.size() - 2] = data[data.size() - 3];
        data[data.size() - 3] = temp;
    }

    // Stack validation
    bool is_valid() const {
        return size() <= capacity;
    }

    // Memory management
    void shrink_to_fit() {
        data.shrink_to_fit();
        capacity = data.capacity();
    }

private:
    void resize_capacity() {
        capacity = static_cast<size_t>(capacity * 1.5);
        data.reserve(capacity);
    }
};

// Type aliases for common use cases
using IntStack = Stack<int>;
using DoubleStack = Stack<double>;
using StringStack = Stack<std::string>;
using CharStack = Stack<char>;

// Utility functions for stack operations
template<typename T>
void reverse_stack(Stack<T>& stack) {
    Stack<T> temp;
    while (!stack.empty()) {
        temp.push(stack.top());
        stack.pop();
    }
    stack = std::move(temp);
}

template<typename T>
Stack<T> copy_stack(const Stack<T>& stack) {
    return Stack<T>(stack);
}

template<typename T>
bool are_stacks_equal(const Stack<T>& stack1, const Stack<T>& stack2) {
    if (stack1.size() != stack2.size()) {
        return false;
    }
    
    Stack<T> temp1 = copy_stack(stack1);
    Stack<T> temp2 = copy_stack(stack2);
    
    while (!temp1.empty() && !temp2.empty()) {
        if (temp1.top() != temp2.top()) {
            return false;
        }
        temp1.pop();
        temp2.pop();
    }
    
    return temp1.empty() && temp2.empty();
}

} // namespace structures
} // namespace dsa
