#pragma once

#include <iostream>

template <typename T>
class LinkedList {
protected:
    struct Node {
        T data;
        Node* next;

        Node(const T& val) : data(val), next(nullptr) {}
    };

    Node* head;
    size_t length;

public:
    LinkedList() : head(nullptr), length(0) {}
    virtual ~LinkedList() {
        clear();
    }

    virtual void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next)
                current = current->next;
            current->next = newNode;
        }
        ++length;
    }

    virtual void print() const {
        Node* current = head;
        while (current) {
            std::cout << current->data << " -> ";
            current = current->next;
        }
        std::cout << "null\n";
    }

    void clear() {
        Node* current = head;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        length = 0;
    }

    size_t size() const { return length; }
    bool empty() const { return head == nullptr; }
};
