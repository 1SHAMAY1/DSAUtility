#pragma once

#include <iostream>
#include <stdexcept>

template <typename T>
class CircularLinkedList {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& val) : data(val), next(nullptr) {}
    };

    Node* head;
    size_t length;

public:
    CircularLinkedList() : head(nullptr), length(0) {}

    ~CircularLinkedList() {
        while (!empty()) {
            pop_front();
        }
    }

    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = newNode;
            head->next = head;
        } else {
            Node* current = head;
            while (current->next != head)
                current = current->next;
            current->next = newNode;
            newNode->next = head;
        }
        ++length;
    }

    void push_front(const T& value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = newNode;
            head->next = head;
        } else {
            Node* tail = head;
            while (tail->next != head)
                tail = tail->next;

            newNode->next = head;
            tail->next = newNode;
            head = newNode;
        }
        ++length;
    }

    void pop_front() {
        if (!head)
            return;

        if (head->next == head) {
            delete head;
            head = nullptr;
        } else {
            Node* last = head;
            while (last->next != head)
                last = last->next;

            Node* temp = head;
            head = head->next;
            last->next = head;
            delete temp;
        }
        --length;
    }

    T front() const {
        if (!head)
            throw std::out_of_range("CircularLinkedList is empty");
        return head->data;
    }

    bool empty() const {
        return head == nullptr;
    }

    size_t size() const {
        return length;
    }

    void print() const {
        if (!head) {
            std::cout << "null\n";
            return;
        }

        Node* current = head;
        do {
            std::cout << current->data << " -> ";
            current = current->next;
        } while (current != head);
        std::cout << "(head)\n";
    }
};
