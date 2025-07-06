#pragma once
#include "../array/Array.hpp"
#include "IQueue.hpp"

template<typename T>
class ArrayQueue : public IQueue<T> {
private:
    Array<T> data;
public:
    void enqueue(const T& value) override {
        data.add(value);
    }

    void dequeue() override {
        if (!data.empty())
            data.removeAt(0); 
    }

    T front() const override {
        return data[0];
    }

    bool isEmpty() const override {
        return data.empty();
    }

    void print() const override {
        data.print();
    }
};
