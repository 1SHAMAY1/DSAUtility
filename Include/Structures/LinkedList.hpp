#pragma once
#include <memory>
#include <stdexcept>
#include <iostream>
#include <initializer_list>
#include "../Utils/Print.hpp"
#include "../Utils/Logger.hpp"

namespace dsa {
namespace structures {

/**
 * @brief Node structure for singly linked list
 */
template<typename T>
struct ListNode {
    T data;
    std::unique_ptr<ListNode<T>> next;
    
    ListNode(const T& value) : data(value), next(nullptr) {}
    ListNode(T&& value) : data(std::move(value)), next(nullptr) {}
};

/**
 * @brief Singly Linked List Data Structure
 */
template<typename T>
class LinkedList {
private:
    std::unique_ptr<ListNode<T>> head;
    ListNode<T>* tail;
    size_t size_;

public:
    // Constructors
    LinkedList() : head(nullptr), tail(nullptr), size_(0) {}
    
    LinkedList(std::initializer_list<T> init_list) : head(nullptr), tail(nullptr), size_(0) {
        for (const auto& item : init_list) {
            push_back(item);
        }
    }
    
    LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), size_(0) {
        ListNode<T>* current = other.head.get();
        while (current) {
            push_back(current->data);
            current = current->next.get();
        }
    }
    
    LinkedList(LinkedList&& other) noexcept 
        : head(std::move(other.head)), tail(other.tail), size_(other.size_) {
        other.tail = nullptr;
        other.size_ = 0;
    }
    
    // Destructor
    ~LinkedList() = default;
    
    // Assignment operators
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            ListNode<T>* current = other.head.get();
            while (current) {
                push_back(current->data);
                current = current->next.get();
            }
        }
        return *this;
    }
    
    LinkedList& operator=(LinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            head = std::move(other.head);
            tail = other.tail;
            size_ = other.size_;
            other.tail = nullptr;
            other.size_ = 0;
        }
        return *this;
    }
    
    // Capacity
    bool empty() const { return size_ == 0; }
    size_t size() const { return size_; }
    
    // Element access
    T& front() {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        return head->data;
    }
    
    const T& front() const {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        return head->data;
    }
    
    T& back() {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        return tail->data;
    }
    
    const T& back() const {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        return tail->data;
    }
    
    // Modifiers
    void clear() {
        head.reset();
        tail = nullptr;
        size_ = 0;
    }
    
    void push_front(const T& value) {
        auto new_node = std::make_unique<ListNode<T>>(value);
        new_node->next = std::move(head);
        head = std::move(new_node);
        if (!tail) {
            tail = head.get();
        }
        size_++;
    }
    
    void push_front(T&& value) {
        auto new_node = std::make_unique<ListNode<T>>(std::move(value));
        new_node->next = std::move(head);
        head = std::move(new_node);
        if (!tail) {
            tail = head.get();
        }
        size_++;
    }
    
    void push_back(const T& value) {
        auto new_node = std::make_unique<ListNode<T>>(value);
        if (empty()) {
            head = std::move(new_node);
            tail = head.get();
        } else {
            tail->next = std::move(new_node);
            tail = tail->next.get();
        }
        size_++;
    }
    
    void push_back(T&& value) {
        auto new_node = std::make_unique<ListNode<T>>(std::move(value));
        if (empty()) {
            head = std::move(new_node);
            tail = head.get();
        } else {
            tail->next = std::move(new_node);
            tail = tail->next.get();
        }
        size_++;
    }
    
    void pop_front() {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        head = std::move(head->next);
        if (!head) {
            tail = nullptr;
        }
        size_--;
    }
    
    void pop_back() {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        if (size_ == 1) {
            head.reset();
            tail = nullptr;
        } else {
            ListNode<T>* current = head.get();
            while (current->next->next) {
                current = current->next.get();
            }
            current->next.reset();
            tail = current;
        }
        size_--;
    }
    
    void insert(size_t position, const T& value) {
        if (position > size_) {
            throw std::out_of_range("Insert position out of bounds");
        }
        if (position == 0) {
            push_front(value);
        } else if (position == size_) {
            push_back(value);
        } else {
            auto new_node = std::make_unique<ListNode<T>>(value);
            ListNode<T>* current = head.get();
            for (size_t i = 0; i < position - 1; ++i) {
                current = current->next.get();
            }
            new_node->next = std::move(current->next);
            current->next = std::move(new_node);
            size_++;
        }
    }
    
    void insert(size_t position, T&& value) {
        if (position > size_) {
            throw std::out_of_range("Insert position out of bounds");
        }
        if (position == 0) {
            push_front(std::move(value));
        } else if (position == size_) {
            push_back(std::move(value));
        } else {
            auto new_node = std::make_unique<ListNode<T>>(std::move(value));
            ListNode<T>* current = head.get();
            for (size_t i = 0; i < position - 1; ++i) {
                current = current->next.get();
            }
            new_node->next = std::move(current->next);
            current->next = std::move(new_node);
            size_++;
        }
    }
    
    void erase(size_t position) {
        if (position >= size_) {
            throw std::out_of_range("Erase position out of bounds");
        }
        if (position == 0) {
            pop_front();
        } else {
            ListNode<T>* current = head.get();
            for (size_t i = 0; i < position - 1; ++i) {
                current = current->next.get();
            }
            if (current->next.get() == tail) {
                tail = current;
            }
            current->next = std::move(current->next->next);
            size_--;
        }
    }
    
    void remove(const T& value) {
        if (empty()) return;
        
        // Remove from front if needed
        while (head && head->data == value) {
            pop_front();
        }
        
        if (empty()) return;
        
        // Remove from middle and end
        ListNode<T>* current = head.get();
        while (current->next) {
            if (current->next->data == value) {
                if (current->next.get() == tail) {
                    tail = current;
                }
                current->next = std::move(current->next->next);
                size_--;
            } else {
                current = current->next.get();
            }
        }
    }
    
    // Search operations
    bool contains(const T& value) const {
        ListNode<T>* current = head.get();
        while (current) {
            if (current->data == value) {
                return true;
            }
            current = current->next.get();
        }
        return false;
    }
    
    size_t find(const T& value) const {
        ListNode<T>* current = head.get();
        size_t index = 0;
        while (current) {
            if (current->data == value) {
                return index;
            }
            current = current->next.get();
            index++;
        }
        return static_cast<size_t>(-1);
    }
    
    size_t count(const T& value) const {
        size_t count = 0;
        ListNode<T>* current = head.get();
        while (current) {
            if (current->data == value) {
                count++;
            }
            current = current->next.get();
        }
        return count;
    }
    
    // List manipulation
    void reverse() {
        if (size_ <= 1) return;
        
        ListNode<T>* prev = nullptr;
        ListNode<T>* current = head.get();
        ListNode<T>* next = nullptr;
        
        tail = head.get();
        
        while (current) {
            next = current->next.get();
            current->next.reset(prev);
            prev = current;
            current = next;
        }
        
        head.reset(prev);
    }
    
    void unique() {
        if (size_ <= 1) return;
        
        ListNode<T>* current = head.get();
        while (current->next) {
            if (current->data == current->next->data) {
                if (current->next.get() == tail) {
                    tail = current;
                }
                current->next = std::move(current->next->next);
                size_--;
            } else {
                current = current->next.get();
            }
        }
    }
    
    void sort() {
        if (size_ <= 1) return;
        
        // Simple bubble sort for demonstration
        bool swapped;
        do {
            swapped = false;
            ListNode<T>* current = head.get();
            ListNode<T>* prev = nullptr;
            
            while (current->next) {
                if (current->data > current->next->data) {
                    // Swap data
                    std::swap(current->data, current->next->data);
                    swapped = true;
                }
                prev = current;
                current = current->next.get();
            }
        } while (swapped);
    }
    
    template<typename Compare>
    void sort(Compare comp) {
        if (size_ <= 1) return;
        
        // Simple bubble sort with custom comparator
        bool swapped;
        do {
            swapped = false;
            ListNode<T>* current = head.get();
            ListNode<T>* prev = nullptr;
            
            while (current->next) {
                if (comp(current->next->data, current->data)) {
                    // Swap data
                    std::swap(current->data, current->next->data);
                    swapped = true;
                }
                prev = current;
                current = current->next.get();
            }
        } while (swapped);
    }
    
    // List operations
    void merge(LinkedList& other) {
        while (!other.empty()) {
            push_back(other.front());
            other.pop_front();
        }
    }
    
    void splice(size_t position, LinkedList& other) {
        if (position > size_) {
            throw std::out_of_range("Splice position out of bounds");
        }
        
        if (other.empty()) return;
        
        if (position == 0) {
            // Insert at front
            ListNode<T>* other_tail = other.tail;
            other.tail->next = std::move(head);
            head = std::move(other.head);
            other.tail = nullptr;
            other.size_ = 0;
        } else {
            // Insert at position
            ListNode<T>* current = head.get();
            for (size_t i = 0; i < position - 1; ++i) {
                current = current->next.get();
            }
            
            ListNode<T>* other_tail = other.tail;
            other.tail->next = std::move(current->next);
            current->next = std::move(other.head);
            other.tail = nullptr;
            other.size_ = 0;
        }
        
        size_ += other.size_;
    }
    
    // Comparison operators
    bool operator==(const LinkedList& other) const {
        if (size_ != other.size_) return false;
        
        ListNode<T>* current1 = head.get();
        ListNode<T>* current2 = other.head.get();
        
        while (current1 && current2) {
            if (current1->data != current2->data) {
                return false;
            }
            current1 = current1->next.get();
            current2 = current2->next.get();
        }
        
        return true;
    }
    
    bool operator!=(const LinkedList& other) const {
        return !(*this == other);
    }
    
    bool operator<(const LinkedList& other) const {
        ListNode<T>* current1 = head.get();
        ListNode<T>* current2 = other.head.get();
        
        while (current1 && current2) {
            if (current1->data < current2->data) return true;
            if (current1->data > current2->data) return false;
            current1 = current1->next.get();
            current2 = current2->next.get();
        }
        
        return size_ < other.size_;
    }
    
    bool operator<=(const LinkedList& other) const {
        return (*this < other) || (*this == other);
    }
    
    bool operator>(const LinkedList& other) const {
        return other < *this;
    }
    
    bool operator>=(const LinkedList& other) const {
        return !(*this < other);
    }
    
    // Utility functions
    void print() const {
        std::cout << "LinkedList: ";
        ListNode<T>* current = head.get();
        while (current) {
            std::cout << current->data;
            if (current->next) {
                std::cout << " -> ";
            }
            current = current->next.get();
        }
        std::cout << std::endl;
    }
    
    std::string to_string() const {
        std::stringstream ss;
        ss << "LinkedList[size=" << size_ << "]";
        return ss.str();
    }
    
    // Iterator support (basic implementation)
    class Iterator {
    private:
        ListNode<T>* current;
        
    public:
        Iterator(ListNode<T>* node) : current(node) {}
        
        T& operator*() { return current->data; }
        const T& operator*() const { return current->data; }
        
        Iterator& operator++() {
            current = current->next.get();
            return *this;
        }
        
        Iterator operator++(int) {
            Iterator temp = *this;
            current = current->next.get();
            return temp;
        }
        
        bool operator==(const Iterator& other) const {
            return current == other.current;
        }
        
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };
    
    Iterator begin() { return Iterator(head.get()); }
    Iterator end() { return Iterator(nullptr); }
    Iterator begin() const { return Iterator(head.get()); }
    Iterator end() const { return Iterator(nullptr); }
};

// Type aliases for common use cases
using IntLinkedList = LinkedList<int>;
using DoubleLinkedList = LinkedList<double>;
using StringLinkedList = LinkedList<std::string>;

} // namespace structures
} // namespace dsa
