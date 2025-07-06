#pragma once
#include "Queue.hpp"
#include <vector>
#include <stdexcept>

namespace dsa {
namespace structures {

/**
 * @brief Circular Queue Data Structure
 */
template<typename T>
class CircularQueue : public Queue<T> {
private:
    std::vector<T> buffer;
    size_t front_index;
    size_t rear_index;
    size_t count_;

public:
    // Constructors
    CircularQueue() : Queue<T>(), front_index(0), rear_index(0), count_(0) {}
    
    explicit CircularQueue(size_t capacity) 
        : Queue<T>(capacity), buffer(capacity), front_index(0), rear_index(0), count_(0) {}
    
    CircularQueue(std::initializer_list<T> init_list) 
        : Queue<T>(init_list.size()), front_index(0), rear_index(0), count_(0) {
        buffer.resize(init_list.size());
        for (const auto& item : init_list) {
            push(item);
        }
    }
    
    CircularQueue(const CircularQueue& other) 
        : Queue<T>(other), buffer(other.buffer), front_index(other.front_index), 
          rear_index(other.rear_index), count_(other.count_) {}
    
    CircularQueue(CircularQueue&& other) noexcept 
        : Queue<T>(std::move(other)), buffer(std::move(other.buffer)), 
          front_index(other.front_index), rear_index(other.rear_index), count_(other.count_) {
        other.front_index = 0;
        other.rear_index = 0;
        other.count_ = 0;
    }
    
    // Destructor
    ~CircularQueue() = default;
    
    // Assignment operators
    CircularQueue& operator=(const CircularQueue& other) {
        if (this != &other) {
            Queue<T>::operator=(other);
            buffer = other.buffer;
            front_index = other.front_index;
            rear_index = other.rear_index;
            count_ = other.count_;
        }
        return *this;
    }
    
    CircularQueue& operator=(CircularQueue&& other) noexcept {
        if (this != &other) {
            Queue<T>::operator=(std::move(other));
            buffer = std::move(other.buffer);
            front_index = other.front_index;
            rear_index = other.rear_index;
            count_ = other.count_;
            other.front_index = 0;
            other.rear_index = 0;
            other.count_ = 0;
        }
        return *this;
    }
    
    // Capacity
    bool empty() const override { return count_ == 0; }
    size_t size() const override { return count_; }
    bool full() const override { return count_ == buffer.size(); }
    
    // Element access
    T& front() override {
        if (empty()) {
            throw std::out_of_range("CircularQueue is empty");
        }
        return buffer[front_index];
    }
    
    const T& front() const override {
        if (empty()) {
            throw std::out_of_range("CircularQueue is empty");
        }
        return buffer[front_index];
    }
    
    T& back() override {
        if (empty()) {
            throw std::out_of_range("CircularQueue is empty");
        }
        return buffer[(rear_index - 1 + buffer.size()) % buffer.size()];
    }
    
    const T& back() const override {
        if (empty()) {
            throw std::out_of_range("CircularQueue is empty");
        }
        return buffer[(rear_index - 1 + buffer.size()) % buffer.size()];
    }
    
    // Modifiers
    void push(const T& value) override {
        if (full()) {
            throw std::overflow_error("CircularQueue is full");
        }
        buffer[rear_index] = value;
        rear_index = (rear_index + 1) % buffer.size();
        count_++;
    }
    
    void push(T&& value) override {
        if (full()) {
            throw std::overflow_error("CircularQueue is full");
        }
        buffer[rear_index] = std::move(value);
        rear_index = (rear_index + 1) % buffer.size();
        count_++;
    }
    
    void pop() override {
        if (empty()) {
            throw std::out_of_range("CircularQueue is empty");
        }
        front_index = (front_index + 1) % buffer.size();
        count_--;
    }
    
    T pop_and_get() override {
        if (empty()) {
            throw std::out_of_range("CircularQueue is empty");
        }
        T value = std::move(buffer[front_index]);
        front_index = (front_index + 1) % buffer.size();
        count_--;
        return value;
    }
    
    template<typename... Args>
    void emplace(Args&&... args) {
        if (full()) {
            throw std::overflow_error("CircularQueue is full");
        }
        buffer[rear_index] = T(std::forward<Args>(args)...);
        rear_index = (rear_index + 1) % buffer.size();
        count_++;
    }
    
    void clear() override {
        front_index = 0;
        rear_index = 0;
        count_ = 0;
    }
    
    void resize(size_t new_capacity) override {
        if (new_capacity < count_) {
            throw std::invalid_argument("New capacity is less than current size");
        }
        
        std::vector<T> new_buffer(new_capacity);
        size_t new_count = 0;
        
        // Copy elements to new buffer
        while (!empty() && new_count < new_capacity) {
            new_buffer[new_count++] = pop_and_get();
        }
        
        buffer = std::move(new_buffer);
        front_index = 0;
        rear_index = new_count;
        count_ = new_count;
        this->capacity_ = new_capacity;
    }
    
    // CircularQueue-specific operations
    void rotate(size_t positions) {
        if (empty() || positions == 0) return;
        
        positions %= count_;
        for (size_t i = 0; i < positions; ++i) {
            T value = pop_and_get();
            push(value);
        }
    }
    
    void rotate_reverse(size_t positions) {
        if (empty() || positions == 0) return;
        
        positions %= count_;
        for (size_t i = 0; i < positions; ++i) {
            T value = back();
            pop();
            // Insert at front
            front_index = (front_index - 1 + buffer.size()) % buffer.size();
            buffer[front_index] = std::move(value);
            count_++;
        }
    }
    
    // Access by index (circular)
    T& at(size_t index) {
        if (index >= count_) {
            throw std::out_of_range("Index out of bounds");
        }
        return buffer[(front_index + index) % buffer.size()];
    }
    
    const T& at(size_t index) const {
        if (index >= count_) {
            throw std::out_of_range("Index out of bounds");
        }
        return buffer[(front_index + index) % buffer.size()];
    }
    
    T& operator[](size_t index) { return at(index); }
    const T& operator[](size_t index) const { return at(index); }
    
    // Search operations
    bool contains(const T& value) const override {
        for (size_t i = 0; i < count_; ++i) {
            if (buffer[(front_index + i) % buffer.size()] == value) {
                return true;
            }
        }
        return false;
    }
    
    size_t count(const T& value) const override {
        size_t count = 0;
        for (size_t i = 0; i < count_; ++i) {
            if (buffer[(front_index + i) % buffer.size()] == value) {
                count++;
            }
        }
        return count;
    }
    
    size_t find(const T& value) const override {
        for (size_t i = 0; i < count_; ++i) {
            if (buffer[(front_index + i) % buffer.size()] == value) {
                return i;
            }
        }
        return static_cast<size_t>(-1);
    }
    
    // Utility functions
    void print() const override {
        std::cout << "CircularQueue: ";
        for (size_t i = 0; i < count_; ++i) {
            std::cout << buffer[(front_index + i) % buffer.size()];
            if (i < count_ - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << std::endl;
    }
    
    void print_buffer() const {
        std::cout << "Buffer: ";
        for (size_t i = 0; i < buffer.size(); ++i) {
            if (i == front_index && i == rear_index) {
                std::cout << "[" << buffer[i] << "]"; // Both front and rear
            } else if (i == front_index) {
                std::cout << "[" << buffer[i] << "]"; // Front
            } else if (i == rear_index) {
                std::cout << "(" << buffer[i] << ")"; // Rear
            } else {
                std::cout << buffer[i];
            }
            if (i < buffer.size() - 1) {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
    
    std::string to_string() const override {
        std::stringstream ss;
        ss << "CircularQueue[size=" << count_ << ", capacity=" << buffer.size() 
           << ", front=" << front_index << ", rear=" << rear_index << "]";
        return ss.str();
    }
    
    // Iterator support
    class Iterator {
    private:
        const CircularQueue<T>* queue;
        size_t current_index;
        size_t position;
        
    public:
        Iterator(const CircularQueue<T>* q, size_t pos = 0) 
            : queue(q), current_index(q->front_index), position(pos) {}
        
        T& operator*() { 
            return const_cast<T&>(queue->buffer[current_index]); 
        }
        
        const T& operator*() const { 
            return queue->buffer[current_index]; 
        }
        
        Iterator& operator++() {
            if (position < queue->count_) {
                current_index = (current_index + 1) % queue->buffer.size();
                position++;
            }
            return *this;
        }
        
        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }
        
        bool operator==(const Iterator& other) const {
            return position == other.position;
        }
        
        bool operator!=(const Iterator& other) const {
            return position != other.position;
        }
    };
    
    Iterator begin() { return Iterator(this); }
    Iterator end() { return Iterator(this, count_); }
    Iterator begin() const { return Iterator(this); }
    Iterator end() const { return Iterator(this, count_); }
};

// Type aliases for common use cases
using IntCircularQueue = CircularQueue<int>;
using DoubleCircularQueue = CircularQueue<double>;
using StringCircularQueue = CircularQueue<std::string>;

} // namespace structures
} // namespace dsa
