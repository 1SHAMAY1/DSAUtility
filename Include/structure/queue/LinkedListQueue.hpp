#pragma once
#include "../list/LinkedList.hpp"
#include "IQueue.hpp"

template<typename T>
class LinkedListQueue : public IQueue<T> {
private:
    LinkedList<T> data;
public:
    void enqueue(const T& value) override {
        data.push_back(value);
    }

    void dequeue() override {
        data.pop_front();
    }

    T front() const override {
        return data.front();
    }

    bool isEmpty() const override {
        return data.empty();
    }

    void print() const override {
        data.print();
    }
};
