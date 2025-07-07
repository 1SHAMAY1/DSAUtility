#pragma once
#include "../list/LinkedList.hpp"
#include "IStack.hpp"

template<typename T>
class LinkedListStack : public IStack<T> {
private:
    LinkedList<T> data;
public:
    void push(const T& value) override {
        data.push_front(value);
    }

    void pop() override {
        data.pop_front();
    }

    T top() const override {
        return data.front();
    }

    bool isEmpty() const override {
        return data.empty();
    }

    void print() const override {
        data.print();
    }
};
