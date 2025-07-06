#pragma once

template<typename T>
class IStack {
public:
    virtual void push(const T& value) = 0;
    virtual void pop() = 0;
    virtual T top() const = 0;
    virtual bool isEmpty() const = 0;
    virtual void print() const = 0;
    virtual ~IStack() = default;
};
