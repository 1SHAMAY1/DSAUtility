#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <array>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <iomanip>
#include <sstream>

namespace dsa {
namespace utils {

/**
 * @brief Print utility functions for various data types and containers
 */
class Print {
public:
    // Basic type printing
    template<typename T>
    static void print(const T& value) {
        std::cout << value;
    }

    template<typename T>
    static void println(const T& value) {
        std::cout << value << std::endl;
    }

    // Array printing
    template<typename T, size_t N>
    static void printArray(const T (&arr)[N], const std::string& separator = " ") {
        for (size_t i = 0; i < N; ++i) {
            std::cout << arr[i];
            if (i < N - 1) std::cout << separator;
        }
    }

    template<typename T, size_t N>
    static void printArray(const std::array<T, N>& arr, const std::string& separator = " ") {
        for (size_t i = 0; i < N; ++i) {
            std::cout << arr[i];
            if (i < N - 1) std::cout << separator;
        }
    }

    // Vector printing
    template<typename T>
    static void printVector(const std::vector<T>& vec, const std::string& separator = " ") {
        for (size_t i = 0; i < vec.size(); ++i) {
            std::cout << vec[i];
            if (i < vec.size() - 1) std::cout << separator;
        }
    }

    template<typename T>
    static void printVector(const std::vector<T>& vec, const std::string& prefix, 
                          const std::string& separator = " ") {
        std::cout << prefix;
        printVector(vec, separator);
    }

    // List printing
    template<typename T>
    static void printList(const std::list<T>& lst, const std::string& separator = " ") {
        auto it = lst.begin();
        while (it != lst.end()) {
            std::cout << *it;
            ++it;
            if (it != lst.end()) std::cout << separator;
        }
    }

    // Deque printing
    template<typename T>
    static void printDeque(const std::deque<T>& dq, const std::string& separator = " ") {
        for (size_t i = 0; i < dq.size(); ++i) {
            std::cout << dq[i];
            if (i < dq.size() - 1) std::cout << separator;
        }
    }

    // Set printing
    template<typename T>
    static void printSet(const std::set<T>& set, const std::string& separator = " ") {
        auto it = set.begin();
        while (it != set.end()) {
            std::cout << *it;
            ++it;
            if (it != set.end()) std::cout << separator;
        }
    }

    template<typename T>
    static void printSet(const std::unordered_set<T>& set, const std::string& separator = " ") {
        for (const auto& item : set) {
            std::cout << item << separator;
        }
    }

    // Map printing
    template<typename K, typename V>
    static void printMap(const std::map<K, V>& map, const std::string& separator = " ") {
        for (const auto& pair : map) {
            std::cout << "(" << pair.first << ": " << pair.second << ")" << separator;
        }
    }

    template<typename K, typename V>
    static void printMap(const std::unordered_map<K, V>& map, const std::string& separator = " ") {
        for (const auto& pair : map) {
            std::cout << "(" << pair.first << ": " << pair.second << ")" << separator;
        }
    }

    // Matrix printing
    template<typename T>
    static void printMatrix(const std::vector<std::vector<T>>& matrix, 
                          const std::string& separator = " ") {
        for (const auto& row : matrix) {
            printVector(row, separator);
            std::cout << std::endl;
        }
    }

    // Pretty printing with formatting
    template<typename T>
    static void printFormatted(const T& value, int width = 10) {
        std::cout << std::setw(width) << value;
    }

    template<typename T>
    static void printFormatted(const std::vector<T>& vec, int width = 10) {
        for (const auto& item : vec) {
            printFormatted(item, width);
        }
    }

    // Debug printing
    template<typename T>
    static void printDebug(const T& value, const std::string& label = "") {
        if (!label.empty()) {
            std::cout << label << ": ";
        }
        std::cout << value << std::endl;
    }

    // Separator printing
    static void printSeparator(char separator = '-', int length = 50) {
        std::cout << std::string(length, separator) << std::endl;
    }

    // Title printing
    static void printTitle(const std::string& title, char separator = '=') {
        printSeparator(separator);
        std::cout << title << std::endl;
        printSeparator(separator);
    }

    // New line
    static void newline() {
        std::cout << std::endl;
    }

    // Clear screen (platform dependent)
    static void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
};

// Convenience functions
template<typename T>
void print(const T& value) {
    Print::print(value);
}

template<typename T>
void println(const T& value) {
    Print::println(value);
}

template<typename T>
void printVector(const std::vector<T>& vec, const std::string& separator = " ") {
    Print::printVector(vec, separator);
}

template<typename T>
void printArray(const T* arr, size_t size, const std::string& separator = " ") {
    for (size_t i = 0; i < size; ++i) {
        std::cout << arr[i];
        if (i < size - 1) std::cout << separator;
    }
}

} // namespace utils
} // namespace dsa
