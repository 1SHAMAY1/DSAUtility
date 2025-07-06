#pragma once

#include <iostream>
#include <stdexcept>

template <typename T>
class DoublyLinkedList {
private:
    struct DNode {
        T data;
        DNode* next;
        DNode* prev;

        DNode(const T& val) : data(val), next(nullptr), prev(nullptr) {}
    };

    DNode* head;
    DNode* tail;
    size_t length;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), length(0) {}

    ~DoublyLinkedList() {
        DNode* current = head;
        while (current) {
            DNode* next = current->next;
            delete current;
            current = next;
        }
        head = tail = nullptr;
        length = 0;
    }

    void push_back(const T& value) {
        DNode* newNode = new DNode(value);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        ++length;
    }

    void push_front(const T& value) {
        DNode* newNode = new DNode(value);
        if (!head) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        ++length;
    }

    void pop_front() {
        if (!head) return;
        DNode* temp = head;
        head = head->next;
        if (head)
            head->prev = nullptr;
        else
            tail = nullptr;
        delete temp;
        --length;
    }

    T front() const {
        if (!head)
            throw std::out_of_range("DoublyLinkedList is empty");
        return head->data;
    }

    bool empty() const {
        return head == nullptr;
    }

    size_t size() const {
        return length;
    }

    void print() const {
        DNode* current = head;
        while (current) {
            std::cout << current->data << " <-> ";
            current = current->next;
        }
        std::cout << "null\n";
    }
};
