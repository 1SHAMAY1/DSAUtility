#pragma once
#include "LinkedList.hpp"

namespace dsa {
namespace structures {

/**
 * @brief Doubly Linked List Data Structure
 */
template<typename T>
class DoublyLinkedList : public LinkedList<T> {
public:
    // Constructors
    DoublyLinkedList() : LinkedList<T>() {}
    
    DoublyLinkedList(std::initializer_list<T> init_list) : LinkedList<T>(init_list) {}
    
    DoublyLinkedList(const DoublyLinkedList& other) : LinkedList<T>(other) {}
    
    DoublyLinkedList(DoublyLinkedList&& other) noexcept : LinkedList<T>(std::move(other)) {}
    
    // Destructor
    ~DoublyLinkedList() = default;
    
    // Assignment operators
    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this != &other) {
            LinkedList<T>::operator=(other);
        }
        return *this;
    }
    
    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept {
        if (this != &other) {
            LinkedList<T>::operator=(std::move(other));
        }
        return *this;
    }
    
    // DoublyLinkedList-specific operations
    void insert_before(size_t index, const T& value) {
        if (index > this->size_) {
            throw std::out_of_range("Insert index out of bounds");
        }
        
        if (index == 0) {
            this->push_front(value);
        } else {
            auto current = this->get_node_at(index);
            auto new_node = std::make_unique<ListNode<T>>(value);
            new_node->next = std::move(current->prev->next);
            new_node->prev = current->prev;
            current->prev->next = std::move(new_node);
            current->prev = current->prev->next.get();
            this->size_++;
        }
    }
    
    void insert_before(size_t index, T&& value) {
        if (index > this->size_) {
            throw std::out_of_range("Insert index out of bounds");
        }
        
        if (index == 0) {
            this->push_front(std::move(value));
        } else {
            auto current = this->get_node_at(index);
            auto new_node = std::make_unique<ListNode<T>>(std::move(value));
            new_node->next = std::move(current->prev->next);
            new_node->prev = current->prev;
            current->prev->next = std::move(new_node);
            current->prev = current->prev->next.get();
            this->size_++;
        }
    }
    
    void insert_after(size_t index, const T& value) {
        if (index >= this->size_) {
            throw std::out_of_range("Insert index out of bounds");
        }
        
        if (index == this->size_ - 1) {
            this->push_back(value);
        } else {
            auto current = this->get_node_at(index);
            auto new_node = std::make_unique<ListNode<T>>(value);
            new_node->next = std::move(current->next);
            new_node->prev = current;
            current->next = std::move(new_node);
            if (new_node->next) {
                new_node->next->prev = new_node.get();
            }
            this->size_++;
        }
    }
    
    void insert_after(size_t index, T&& value) {
        if (index >= this->size_) {
            throw std::out_of_range("Insert index out of bounds");
        }
        
        if (index == this->size_ - 1) {
            this->push_back(std::move(value));
        } else {
            auto current = this->get_node_at(index);
            auto new_node = std::make_unique<ListNode<T>>(std::move(value));
            new_node->next = std::move(current->next);
            new_node->prev = current;
            current->next = std::move(new_node);
            if (new_node->next) {
                new_node->next->prev = new_node.get();
            }
            this->size_++;
        }
    }
    
    // Reverse iterator support
    class ReverseIterator {
    private:
        ListNode<T>* current;
        
    public:
        ReverseIterator(ListNode<T>* node) : current(node) {}
        
        T& operator*() { return current->data; }
        const T& operator*() const { return current->data; }
        
        ReverseIterator& operator++() {
            current = current->prev;
            return *this;
        }
        
        ReverseIterator operator++(int) {
            ReverseIterator temp = *this;
            current = current->prev;
            return temp;
        }
        
        ReverseIterator& operator--() {
            current = current->next;
            return *this;
        }
        
        ReverseIterator operator--(int) {
            ReverseIterator temp = *this;
            current = current->next;
            return temp;
        }
        
        bool operator==(const ReverseIterator& other) const {
            return current == other.current;
        }
        
        bool operator!=(const ReverseIterator& other) const {
            return current != other.current;
        }
    };
    
    ReverseIterator rbegin() { return ReverseIterator(this->tail); }
    ReverseIterator rend() { return ReverseIterator(nullptr); }
    ReverseIterator rbegin() const { return ReverseIterator(this->tail); }
    ReverseIterator rend() const { return ReverseIterator(nullptr); }
    
    // Utility functions
    void print() const override {
        std::cout << "DoublyLinkedList: ";
        ListNode<T>* current = this->head.get();
        while (current) {
            std::cout << current->data;
            if (current->next) {
                std::cout << " <-> ";
            }
            current = current->next.get();
        }
        std::cout << std::endl;
    }
    
    std::string to_string() const override {
        std::stringstream ss;
        ss << "DoublyLinkedList[size=" << this->size_ << "]";
        return ss.str();
    }
};

// Type aliases for common use cases
using IntDoublyLinkedList = DoublyLinkedList<int>;
using DoubleDoublyLinkedList = DoublyLinkedList<double>;
using StringDoublyLinkedList = DoublyLinkedList<std::string>;

} // namespace structures
} // namespace dsa
