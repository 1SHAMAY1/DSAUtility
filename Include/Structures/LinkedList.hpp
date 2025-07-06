#pragma once
#include <memory>
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
 * @brief Node structure for LinkedList
 */
template<typename T>
struct ListNode {
    T data;
    std::unique_ptr<ListNode<T>> next;
    ListNode<T>* prev;  // For doubly linked list compatibility
    
    ListNode(const T& value) : data(value), next(nullptr), prev(nullptr) {}
    ListNode(T&& value) : data(std::move(value)), next(nullptr), prev(nullptr) {}
};

/**
 * @brief Base LinkedList Data Structure
 */
template<typename T>
class LinkedList {
protected:
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
        copy_list(other.head.get());
    }
    
    LinkedList(LinkedList&& other) noexcept 
        : head(std::move(other.head)), tail(other.tail), size_(other.size_) {
        other.tail = nullptr;
        other.size_ = 0;
    }
    
    // Destructor
    virtual ~LinkedList() = default;
    
    // Assignment operators
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            copy_list(other.head.get());
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
    virtual T& front() {
        if (empty()) {
            throw std::out_of_range("LinkedList is empty");
        }
        return head->data;
    }
    
    virtual const T& front() const {
        if (empty()) {
            throw std::out_of_range("LinkedList is empty");
        }
        return head->data;
    }
    
    virtual T& back() {
        if (empty()) {
            throw std::out_of_range("LinkedList is empty");
        }
        return tail->data;
    }
    
    virtual const T& back() const {
        if (empty()) {
            throw std::out_of_range("LinkedList is empty");
        }
        return tail->data;
    }
    
    virtual T& at(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of bounds");
        }
        return get_node_at(index)->data;
    }
    
    virtual const T& at(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of bounds");
        }
        return get_node_at(index)->data;
    }
    
    virtual T& operator[](size_t index) { return at(index); }
    virtual const T& operator[](size_t index) const { return at(index); }
    
    // Modifiers
    virtual void push_front(const T& value) {
        auto new_node = std::make_unique<ListNode<T>>(value);
        new_node->next = std::move(head);
        if (head) {
            head->prev = new_node.get();
        }
        head = std::move(new_node);
        if (!tail) {
            tail = head.get();
        }
        size_++;
    }
    
    virtual void push_front(T&& value) {
        auto new_node = std::make_unique<ListNode<T>>(std::move(value));
        new_node->next = std::move(head);
        if (head) {
            head->prev = new_node.get();
        }
        head = std::move(new_node);
        if (!tail) {
            tail = head.get();
        }
        size_++;
    }
    
    virtual void push_back(const T& value) {
        auto new_node = std::make_unique<ListNode<T>>(value);
        new_node->prev = tail;
        
        if (tail) {
            tail->next = std::move(new_node);
            tail = tail->next.get();
        } else {
            head = std::move(new_node);
            tail = head.get();
        }
        size_++;
    }
    
    virtual void push_back(T&& value) {
        auto new_node = std::make_unique<ListNode<T>>(std::move(value));
        new_node->prev = tail;
        
        if (tail) {
            tail->next = std::move(new_node);
            tail = tail->next.get();
        } else {
            head = std::move(new_node);
            tail = head.get();
        }
        size_++;
    }
    
    virtual void pop_front() {
        if (empty()) {
            throw std::out_of_range("LinkedList is empty");
        }
        
        head = std::move(head->next);
        if (head) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }
        size_--;
    }
    
    virtual void pop_back() {
        if (empty()) {
            throw std::out_of_range("LinkedList is empty");
        }
        
        if (head.get() == tail) {
            head.reset();
            tail = nullptr;
        } else {
            tail = tail->prev;
            tail->next.reset();
        }
        size_--;
    }
    
    virtual void insert(size_t index, const T& value) {
        if (index > size_) {
            throw std::out_of_range("Insert index out of bounds");
        }
        
        if (index == 0) {
            push_front(value);
        } else if (index == size_) {
            push_back(value);
        } else {
            auto current = get_node_at(index);
            auto new_node = std::make_unique<ListNode<T>>(value);
            new_node->next = std::move(current->prev->next);
            new_node->prev = current->prev;
            current->prev->next = std::move(new_node);
            current->prev = current->prev->next.get();
            size_++;
        }
    }
    
    virtual void insert(size_t index, T&& value) {
        if (index > size_) {
            throw std::out_of_range("Insert index out of bounds");
        }
        
        if (index == 0) {
            push_front(std::move(value));
        } else if (index == size_) {
            push_back(std::move(value));
        } else {
            auto current = get_node_at(index);
            auto new_node = std::make_unique<ListNode<T>>(std::move(value));
            new_node->next = std::move(current->prev->next);
            new_node->prev = current->prev;
            current->prev->next = std::move(new_node);
            current->prev = current->prev->next.get();
            size_++;
        }
    }
    
    virtual void erase(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Erase index out of bounds");
        }
        
        if (index == 0) {
            pop_front();
        } else if (index == size_ - 1) {
            pop_back();
        } else {
            auto current = get_node_at(index);
            current->prev->next = std::move(current->next);
            if (current->next) {
                current->next->prev = current->prev;
            }
            size_--;
        }
    }
    
    virtual void clear() {
        head.reset();
        tail = nullptr;
        size_ = 0;
    }
    
    template<typename... Args>
    void emplace_front(Args&&... args) {
        auto new_node = std::make_unique<ListNode<T>>(T(std::forward<Args>(args)...));
        new_node->next = std::move(head);
        if (head) {
            head->prev = new_node.get();
        }
        head = std::move(new_node);
        if (!tail) {
            tail = head.get();
        }
        size_++;
    }
    
    template<typename... Args>
    void emplace_back(Args&&... args) {
        auto new_node = std::make_unique<ListNode<T>>(T(std::forward<Args>(args)...));
        new_node->prev = tail;
        
        if (tail) {
            tail->next = std::move(new_node);
            tail = tail->next.get();
        } else {
            head = std::move(new_node);
            tail = head.get();
        }
        size_++;
    }
    
    // List operations
    virtual void reverse() {
        if (size_ <= 1) return;
        
        ListNode<T>* prev = nullptr;
        ListNode<T>* current = head.get();
        tail = head.get();
        
        while (current) {
            ListNode<T>* next = current->next.get();
            current->next.reset();
            current->prev = prev;
            if (prev) {
                prev->next.reset();
                prev->next = std::unique_ptr<ListNode<T>>(current);
            }
            prev = current;
            current = next;
        }
        
        head = std::unique_ptr<ListNode<T>>(prev);
    }
    
    virtual void sort() {
        if (size_ <= 1) return;
        
        std::vector<T> elements;
        for (auto it = begin(); it != end(); ++it) {
            elements.push_back(*it);
        }
        
        std::sort(elements.begin(), elements.end());
        
        clear();
        for (const auto& element : elements) {
            push_back(element);
        }
    }
    
    template<typename Compare>
    void sort(Compare comp) {
        if (size_ <= 1) return;
        
        std::vector<T> elements;
        for (auto it = begin(); it != end(); ++it) {
            elements.push_back(*it);
        }
        
        std::sort(elements.begin(), elements.end(), comp);
        
        clear();
        for (const auto& element : elements) {
            push_back(element);
        }
    }
    
    virtual void unique() {
        if (size_ <= 1) return;
        
        ListNode<T>* current = head.get();
        while (current && current->next) {
            if (current->data == current->next->data) {
                auto temp = std::move(current->next);
                current->next = std::move(temp->next);
                if (current->next) {
                    current->next->prev = current;
                } else {
                    tail = current;
                }
                size_--;
            } else {
                current = current->next.get();
            }
        }
    }
    
    // Search operations
    virtual bool contains(const T& value) const {
        return find(value) != static_cast<size_t>(-1);
    }
    
    virtual size_t find(const T& value) const {
        size_t index = 0;
        ListNode<T>* current = head.get();
        while (current) {
            if (current->data == value) {
                return index;
            }
            current = current->next.get();
            index++;
        }
        return static_cast<size_t>(-1);
    }
    
    virtual size_t count(const T& value) const {
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
    
    // Mathematical operations (for numeric types)
    template<typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, T>::type sum() const {
        T result = T{};
        ListNode<T>* current = head.get();
        while (current) {
            result += current->data;
            current = current->next.get();
        }
        return result;
    }
    
    template<typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, T>::type min() const {
        if (empty()) {
            throw std::out_of_range("LinkedList is empty");
        }
        T min_val = head->data;
        ListNode<T>* current = head->next.get();
        while (current) {
            if (current->data < min_val) {
                min_val = current->data;
            }
            current = current->next.get();
        }
        return min_val;
    }
    
    template<typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, T>::type max() const {
        if (empty()) {
            throw std::out_of_range("LinkedList is empty");
        }
        T max_val = head->data;
        ListNode<T>* current = head->next.get();
        while (current) {
            if (current->data > max_val) {
                max_val = current->data;
            }
            current = current->next.get();
        }
        return max_val;
    }
    
    template<typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, double>::type average() const {
        if (empty()) {
            throw std::out_of_range("LinkedList is empty");
        }
        return static_cast<double>(sum()) / size_;
    }
    
    // Comparison operators
    virtual bool operator==(const LinkedList& other) const {
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
    
    virtual bool operator!=(const LinkedList& other) const {
        return !(*this == other);
    }
    
    // Utility functions
    virtual void print() const {
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
    
    virtual void print_reverse() const {
        std::cout << "LinkedList (reverse): ";
        ListNode<T>* current = tail;
        while (current) {
            std::cout << current->data;
            if (current->prev) {
                std::cout << " -> ";
            }
            current = current->prev;
        }
        std::cout << std::endl;
    }
    
    virtual std::string to_string() const {
        std::stringstream ss;
        ss << "LinkedList[size=" << size_ << "]";
        return ss.str();
    }
    
    // Iterator support
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

protected:
    // Helper functions
    virtual ListNode<T>* get_node_at(size_t index) const {
        if (index >= size_) return nullptr;
        
        ListNode<T>* current = head.get();
        for (size_t i = 0; i < index; ++i) {
            current = current->next.get();
        }
        return current;
    }
    
    virtual void copy_list(ListNode<T>* node) {
        while (node) {
            push_back(node->data);
            node = node->next.get();
        }
    }
};

// Type aliases for common use cases
using IntLinkedList = LinkedList<int>;
using DoubleLinkedList = LinkedList<double>;
using StringLinkedList = LinkedList<std::string>;

} // namespace structures
} // namespace dsa
