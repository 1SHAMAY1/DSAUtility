#pragma once
#include <vector>
#include <functional>
#include <stdexcept>

// Template-based binary heap (min-heap by default)
template<typename T, typename Compare = std::less<T>>
class Heap {
    std::vector<T> data;
    Compare comp;
public:
    Heap() = default;
    explicit Heap(Compare cmp) : comp(cmp) {}

    void push(const T& value) {
        data.push_back(value);
        sift_up(data.size() - 1);
    }
    void pop() {
        if (data.empty()) throw std::out_of_range("Heap is empty");
        std::swap(data.front(), data.back());
        data.pop_back();
        if (!data.empty()) sift_down(0);
    }
    const T& top() const {
        if (data.empty()) throw std::out_of_range("Heap is empty");
        return data.front();
    }
    size_t size() const { return data.size(); }
    bool empty() const { return data.empty(); }
private:
    void sift_up(size_t idx) {
        while (idx > 0) {
            size_t parent = (idx - 1) / 2;
            if (!comp(data[idx], data[parent])) break;
            std::swap(data[idx], data[parent]);
            idx = parent;
        }
    }
    void sift_down(size_t idx) {
        size_t n = data.size();
        while (2 * idx + 1 < n) {
            size_t left = 2 * idx + 1, right = 2 * idx + 2, smallest = idx;
            if (left < n && comp(data[left], data[smallest])) smallest = left;
            if (right < n && comp(data[right], data[smallest])) smallest = right;
            if (smallest == idx) break;
            std::swap(data[idx], data[smallest]);
            idx = smallest;
        }
    }
}; 