#include <iostream>
#include <string>
#include <limits>
#include <algorithm>

#include "structure/array/Array.hpp"
#include "structure/array/ArrayUtils.hpp"
#include "structure/list/LinkedList.hpp"
#include "structure/list/DoublyLinkedList.hpp"
#include "structure/list/CircularLinkedList.hpp"

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

template<typename T>
void inputElements(Array<T>& arr, const std::string& label) {
    int count;
    std::cout << "Enter number of elements: ";
    std::cin >> count;
    clearInput();

    for (int i = 0; i < count; ++i) {
        T value;
        std::cout << label << " " << (i + 1) << ": ";
        std::cin >> value;
        arr.add(value);
    }

    std::cout << "\nArray contents:\n";
    arr.print();
}

template<typename T, typename ListType>
void inputLinkedList(ListType& list, const std::string& label) {
    int count;
    std::cout << "Enter number of elements: ";
    std::cin >> count;
    clearInput();

    for (int i = 0; i < count; ++i) {
        T value;
        std::cout << label << " " << (i + 1) << ": ";
        std::cin >> value;
        list.push_back(value);
    }

    std::cout << "\nList contents:\n";
    list.print();
}

void testArray() {
    std::string type;
    std::cout << "Data type (int/float/string): ";
    std::cin >> type;
    type = toLower(type);

    if (type == "int") {
        Array<int> arr;
        inputElements(arr, "Enter int");
    } else if (type == "float") {
        Array<float> arr;
        inputElements(arr, "Enter float");
    } else if (type == "string") {
        Array<std::string> arr;
        inputElements(arr, "Enter string");
    } else {
        std::cout << "❌ Invalid data type.\n";
    }
}

void testLinkedList() {
    std::string ds, type;
    std::cout << "List type (sll/dll/cll): ";
    std::cin >> ds;
    ds = toLower(ds);

    std::cout << "Data type (int/float/string): ";
    std::cin >> type;
    type = toLower(type);

    if (ds == "sll") {
        if (type == "int") {
            LinkedList<int> list;
            inputLinkedList<int, LinkedList<int>>(list, "Enter int");
        } else if (type == "float") {
            LinkedList<float> list;
            inputLinkedList<float, LinkedList<float>>(list, "Enter float");
        } else if (type == "string") {
            LinkedList<std::string> list;
            inputLinkedList<std::string, LinkedList<std::string>>(list, "Enter string");
        } else {
            std::cout << "❌ Invalid data type.\n";
        }
    } else if (ds == "dll") {
        if (type == "int") {
            DoublyLinkedList<int> list;
            inputLinkedList<int, DoublyLinkedList<int>>(list, "Enter int");
        } else if (type == "float") {
            DoublyLinkedList<float> list;
            inputLinkedList<float, DoublyLinkedList<float>>(list, "Enter float");
        } else if (type == "string") {
            DoublyLinkedList<std::string> list;
            inputLinkedList<std::string, DoublyLinkedList<std::string>>(list, "Enter string");
        } else {
            std::cout << "❌ Invalid data type.\n";
        }
    } else if (ds == "cll") {
        if (type == "int") {
            CircularLinkedList<int> list;
            inputLinkedList<int, CircularLinkedList<int>>(list, "Enter int");
        } else if (type == "float") {
            CircularLinkedList<float> list;
            inputLinkedList<float, CircularLinkedList<float>>(list, "Enter float");
        } else if (type == "string") {
            CircularLinkedList<std::string> list;
            inputLinkedList<std::string, CircularLinkedList<std::string>>(list, "Enter string");
        } else {
            std::cout << "❌ Invalid data type.\n";
        }
    } else {
        std::cout << "❌ Invalid list type.\n";
    }
}

int main() {
    std::string choice;
    std::cout << "Test structure (array/linkedlist): ";
    std::cin >> choice;
    choice = toLower(choice);

    if (choice == "array") {
        testArray();
    } else if (choice == "linkedlist") {
        testLinkedList();
    } else {
        std::cout << "❌ Invalid structure choice.\n";
    }

    return 0;
}
