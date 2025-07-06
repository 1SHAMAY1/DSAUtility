#pragma once

template<typename T>
class IQueue {
public:
    virtual void enqueue(const T& value) = 0;
    virtual void dequeue() = 0;
    virtual T front() const = 0;
    virtual bool isEmpty() const = 0;
    virtual void print() const = 0;
    virtual ~IQueue() = default;
};
