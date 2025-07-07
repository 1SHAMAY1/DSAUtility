#pragma once
#include "../array/Array.hpp"
#include "IStack.hpp"
#include <iostream>

template<typename T>
class ArrayStack : public IStack<T> {
private:
    Array<T> data;
public:
    void push(const T& value) override {
        data.add(value);
    }

    void pop() override {
        if (!data.empty()) data.removeLast();
    }

    T top() const override {
        return data.back();
    }

    bool isEmpty() const override {
        return data.empty();
    }

    void print() const override {
        data.print();
    }
};
