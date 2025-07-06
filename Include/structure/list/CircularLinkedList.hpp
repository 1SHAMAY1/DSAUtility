#pragma once

#include "LinkedList.hpp"

template <typename T>
class CircularLinkedList : public LinkedList<T> {
protected:
    using Node = typename LinkedList<T>::Node;

public:
    void push_back(const T& value) override {
        Node* newNode = new Node(value);
        if (!this->head) {
            this->head = newNode;
            newNode->next = this->head;
        } else {
            Node* current = this->head;
            while (current->next != this->head)
                current = current->next;
            current->next = newNode;
            newNode->next = this->head;
        }
        this->length++;
    }

    void print() const override {
        if (!this->head) {
            std::cout << "null\n";
            return;
        }

        Node* current = this->head;
        do {
            std::cout << current->data << " -> ";
            current = current->next;
        } while (current != this->head);
        std::cout << "(head)\n";
    }
};
