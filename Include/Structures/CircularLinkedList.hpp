#pragma once
#include "LinkedList.hpp"

namespace dsa {
namespace structures {

/**
 * @brief Circular Linked List Data Structure
 */
template<typename T>
class CircularLinkedList : public LinkedList<T> {
public:
    // Constructors
    CircularLinkedList() : LinkedList<T>() {}
    
    CircularLinkedList(std::initializer_list<T> init_list) : LinkedList<T>(init_list) {
        make_circular();
    }
    
    CircularLinkedList(const CircularLinkedList& other) : LinkedList<T>(other) {
        make_circular();
    }
    
    CircularLinkedList(CircularLinkedList&& other) noexcept : LinkedList<T>(std::move(other)) {
        make_circular();
    }
    
    // Destructor
    ~CircularLinkedList() = default;
    
    // Assignment operators
    CircularLinkedList& operator=(const CircularLinkedList& other) {
        if (this != &other) {
            LinkedList<T>::operator=(other);
            make_circular();
        }
        return *this;
    }
    
    CircularLinkedList& operator=(CircularLinkedList&& other) noexcept {
        if (this != &other) {
            LinkedList<T>::operator=(std::move(other));
            make_circular();
        }
        return *this;
    }
    
    // Override modifiers to maintain circular structure
    void push_front(const T& value) override {
        LinkedList<T>::push_front(value);
        make_circular();
    }
    
    void push_front(T&& value) override {
        LinkedList<T>::push_front(std::move(value));
        make_circular();
    }
    
    void push_back(const T& value) override {
        LinkedList<T>::push_back(value);
        make_circular();
    }
    
    void push_back(T&& value) override {
        LinkedList<T>::push_back(std::move(value));
        make_circular();
    }
    
    void pop_front() override {
        LinkedList<T>::pop_front();
        make_circular();
    }
    
    void pop_back() override {
        LinkedList<T>::pop_back();
        make_circular();
    }
    
    void insert(size_t index, const T& value) override {
        LinkedList<T>::insert(index, value);
        make_circular();
    }
    
    void insert(size_t index, T&& value) override {
        LinkedList<T>::insert(index, std::move(value));
        make_circular();
    }
    
    void erase(size_t index) override {
        LinkedList<T>::erase(index);
        make_circular();
    }
    
    void clear() override {
        LinkedList<T>::clear();
    }
    
    // CircularLinkedList-specific operations
    void rotate(size_t positions) {
        if (this->empty() || positions == 0) return;
        
        positions %= this->size_;
        for (size_t i = 0; i < positions; ++i) {
            T value = this->front();
            this->pop_front();
            this->push_back(value);
        }
    }
    
    void rotate_reverse(size_t positions) {
        if (this->empty() || positions == 0) return;
        
        positions %= this->size_;
        for (size_t i = 0; i < positions; ++i) {
            T value = this->back();
            this->pop_back();
            this->push_front(value);
        }
    }
    
    // Access by index (circular)
    T& at(size_t index) override {
        if (this->empty()) {
            throw std::out_of_range("CircularLinkedList is empty");
        }
        index %= this->size_;
        return LinkedList<T>::at(index);
    }
    
    const T& at(size_t index) const override {
        if (this->empty()) {
            throw std::out_of_range("CircularLinkedList is empty");
        }
        index %= this->size_;
        return LinkedList<T>::at(index);
    }
    
    T& operator[](size_t index) override { return at(index); }
    const T& operator[](size_t index) const override { return at(index); }
    
    // Circular iterator support
    class CircularIterator {
    private:
        ListNode<T>* current;
        ListNode<T>* start;
        size_t position;
        size_t max_positions;
        
    public:
        CircularIterator(ListNode<T>* node, size_t max_pos = 0) 
            : current(node), start(node), position(0), max_positions(max_pos) {}
        
        T& operator*() { return current->data; }
        const T& operator*() const { return current->data; }
        
        CircularIterator& operator++() {
            if (current) {
                current = current->next.get();
                position++;
                if (max_positions > 0 && position >= max_positions) {
                    current = nullptr; // End iteration
                }
            }
            return *this;
        }
        
        CircularIterator operator++(int) {
            CircularIterator temp = *this;
            ++(*this);
            return temp;
        }
        
        bool operator==(const CircularIterator& other) const {
            return current == other.current;
        }
        
        bool operator!=(const CircularIterator& other) const {
            return current != other.current;
        }
    };
    
    CircularIterator begin() { return CircularIterator(this->head.get(), this->size_); }
    CircularIterator end() { return CircularIterator(nullptr); }
    CircularIterator begin() const { return CircularIterator(this->head.get(), this->size_); }
    CircularIterator end() const { return CircularIterator(nullptr); }
    
    // Utility functions
    void print() const override {
        if (this->empty()) {
            std::cout << "CircularLinkedList: (empty)" << std::endl;
            return;
        }
        
        std::cout << "CircularLinkedList: ";
        ListNode<T>* current = this->head.get();
        size_t count = 0;
        
        while (current && count < this->size_) {
            std::cout << current->data;
            if (count < this->size_ - 1) {
                std::cout << " -> ";
            }
            current = current->next.get();
            count++;
        }
        std::cout << " -> " << this->head->data << " (circular)" << std::endl;
    }
    
    void print_circular() const {
        if (this->empty()) {
            std::cout << "CircularLinkedList: (empty)" << std::endl;
            return;
        }
        
        std::cout << "CircularLinkedList (circular): ";
        ListNode<T>* current = this->head.get();
        size_t count = 0;
        
        do {
            std::cout << current->data;
            if (count < this->size_ - 1) {
                std::cout << " -> ";
            }
            current = current->next.get();
            count++;
        } while (current != this->head.get() && count < this->size_);
        
        std::cout << " -> " << this->head->data << std::endl;
    }
    
    std::string to_string() const override {
        std::stringstream ss;
        ss << "CircularLinkedList[size=" << this->size_ << "]";
        return ss.str();
    }
    
    // CircularLinkedList-specific properties
    bool is_circular() const {
        if (this->empty()) return false;
        return this->tail && this->tail->next.get() == this->head.get();
    }
    
    size_t cycle_length() const {
        if (this->empty()) return 0;
        return this->size_;
    }

private:
    void make_circular() {
        if (this->tail && this->head) {
            this->tail->next = std::unique_ptr<ListNode<T>>(this->head.get());
            this->head->prev = this->tail;
        }
    }
};

// Type aliases for common use cases
using IntCircularLinkedList = CircularLinkedList<int>;
using DoubleCircularLinkedList = CircularLinkedList<double>;
using StringCircularLinkedList = CircularLinkedList<std::string>;

} // namespace structures
} // namespace dsa
