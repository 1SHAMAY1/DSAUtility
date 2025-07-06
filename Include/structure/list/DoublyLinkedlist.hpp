#pragma once

#include "LinkedList.hpp"

template <typename T>
class DoublyLinkedList : public LinkedList<T> {
protected:
    struct DNode {
        T data;
        DNode* next;
        DNode* prev;

        DNode(const T& val) : data(val), next(nullptr), prev(nullptr) {}
    };

    DNode* dHead;
    DNode* dTail;

public:
    DoublyLinkedList() : dHead(nullptr), dTail(nullptr) {}

    ~DoublyLinkedList() {
        DNode* current = dHead;
        while (current) {
            DNode* next = current->next;
            delete current;
            current = next;
        }
    }

    void push_back(const T& value) override {
        DNode* newNode = new DNode(value);
        if (!dHead) {
            dHead = dTail = newNode;
        } else {
            dTail->next = newNode;
            newNode->prev = dTail;
            dTail = newNode;
        }
        this->length++;
    }

    void print() const override {
        DNode* current = dHead;
        while (current) {
            std::cout << current->data << " <-> ";
            current = current->next;
        }
        std::cout << "null\n";
    }
};
