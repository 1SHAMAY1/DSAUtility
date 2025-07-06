#include <iostream>
#include <string>
#include <limits>
#include <algorithm>

// Array
#include "structure/array/Array.hpp"
#include "structure/array/ArrayUtils.hpp"

// Linked Lists
#include "structure/list/LinkedList.hpp"
#include "structure/list/DoublyLinkedList.hpp"
#include "structure/list/CircularLinkedList.hpp"

// Stack
#include "structure/stack/IStack.hpp"
#include "structure/stack/ArrayStack.hpp"
#include "structure/stack/LinkedListStack.hpp"

// Queue
#include "structure/queue/IQueue.hpp"
#include "structure/queue/ArrayQueue.hpp"
#include "structure/queue/LinkedListQueue.hpp"

using namespace std;

// === Utility ===
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

// === Input Handlers ===
template<typename T>
void inputElements(Array<T>& arr, const string& label) {
    int count;
    cout << "Enter number of elements: ";
    cin >> count;
    clearInput();

    for (int i = 0; i < count; ++i) {
        T value;
        cout << label << " " << (i + 1) << ": ";
        cin >> value;
        arr.add(value);
    }

    cout << "\nArray contents:\n";
    arr.print();
}

template<typename T>
void inputSinglyLinkedList(LinkedList<T>& list, const string& label) {
    int count;
    cout << "Enter number of elements: ";
    cin >> count;
    clearInput();

    for (int i = 0; i < count; ++i) {
        T value;
        cout << label << " " << (i + 1) << ": ";
        cin >> value;
        list.push_back(value);
    }

    cout << "\nSingly Linked List contents:\n";
    list.print();
}

template<typename T>
void inputDoublyLinkedList(DoublyLinkedList<T>& list, const string& label) {
    int count;
    cout << "Enter number of elements: ";
    cin >> count;
    clearInput();

    for (int i = 0; i < count; ++i) {
        T value;
        cout << label << " " << (i + 1) << ": ";
        cin >> value;
        list.push_back(value);
    }

    cout << "\nDoubly Linked List contents:\n";
    list.print();
}

template<typename T>
void inputCircularLinkedList(CircularLinkedList<T>& list, const string& label) {
    int count;
    cout << "Enter number of elements: ";
    cin >> count;
    clearInput();

    for (int i = 0; i < count; ++i) {
        T value;
        cout << label << " " << (i + 1) << ": ";
        cin >> value;
        list.push_back(value);
    }

    cout << "\nCircular Linked List contents:\n";
    list.print();
}

template<typename T>
void inputStack(IStack<T>& stack, const string& label) {
    int count;
    cout << "Enter number of elements to push: ";
    cin >> count;
    clearInput();

    for (int i = 0; i < count; ++i) {
        T value;
        cout << label << " " << (i + 1) << ": ";
        cin >> value;
        stack.push(value);
    }

    cout << "\nStack contents:\n";
    stack.print();
}

template<typename T>
void inputQueue(IQueue<T>& queue, const string& label) {
    int count;
    cout << "Enter number of elements to enqueue: ";
    cin >> count;
    clearInput();

    for (int i = 0; i < count; ++i) {
        T value;
        cout << label << " " << (i + 1) << ": ";
        cin >> value;
        queue.enqueue(value);
    }

    cout << "\nQueue contents:\n";
    queue.print();
}

// === Structure Tests ===

void testArray() {
    int type;
    cout << "Data type:\n 1. int\n 2. float\n 3. string\nChoose (1-3): ";
    cin >> type;

    switch (type) {
        case 1: {
            Array<int> arr;
            inputElements(arr, "Enter int");
            break;
        }
        case 2: {
            Array<float> arr;
            inputElements(arr, "Enter float");
            break;
        }
        case 3: {
            Array<string> arr;
            inputElements(arr, "Enter string");
            break;
        }
        default:
            cout << "Invalid data type.\n";
            exit(1);
    }
}

void testLinkedList() {
    int listType, type;
    cout << "Linked List Type:\n 1. Singly\n 2. Doubly\n 3. Circular\nChoose (1-3): ";
    cin >> listType;

    cout << "Data type:\n 1. int\n 2. float\n 3. string\nChoose (1-3): ";
    cin >> type;

    switch (listType) {
        case 1:
            switch (type) {
                case 1: {
                    LinkedList<int> list;
                    inputSinglyLinkedList(list, "Enter int");
                    break;
                }
                case 2: {
                    LinkedList<float> list;
                    inputSinglyLinkedList(list, "Enter float");
                    break;
                }
                case 3: {
                    LinkedList<string> list;
                    inputSinglyLinkedList(list, "Enter string");
                    break;
                }
                default: cout << "Invalid data type.\n"; exit(1);
            }
            break;
        case 2:
            switch (type) {
                case 1: {
                    DoublyLinkedList<int> list;
                    inputDoublyLinkedList(list, "Enter int");
                    break;
                }
                case 2: {
                    DoublyLinkedList<float> list;
                    inputDoublyLinkedList(list, "Enter float");
                    break;
                }
                case 3: {
                    DoublyLinkedList<string> list;
                    inputDoublyLinkedList(list, "Enter string");
                    break;
                }
                default: cout << "Invalid data type.\n"; exit(1);
            }
            break;
        case 3:
            switch (type) {
                case 1: {
                    CircularLinkedList<int> list;
                    inputCircularLinkedList(list, "Enter int");
                    break;
                }
                case 2: {
                    CircularLinkedList<float> list;
                    inputCircularLinkedList(list, "Enter float");
                    break;
                }
                case 3: {
                    CircularLinkedList<string> list;
                    inputCircularLinkedList(list, "Enter string");
                    break;
                }
                default: cout << "Invalid data type.\n"; exit(1);
            }
            break;
        default: cout << "Invalid list type.\n"; exit(1);
    }
}

void testStack() {
    int impl, type;
    cout << "Stack type:\n 1. Array Stack\n 2. Linked List Stack\nChoose (1-2): ";
    cin >> impl;

    cout << "Data type:\n 1. int\n 2. float\n 3. string\nChoose (1-3): ";
    cin >> type;

    switch (impl) {
        case 1:
            switch (type) {
                case 1: { ArrayStack<int> s; inputStack(s, "Push int"); break; }
                case 2: { ArrayStack<float> s; inputStack(s, "Push float"); break; }
                case 3: { ArrayStack<string> s; inputStack(s, "Push string"); break; }
                default: cout << "Invalid data type.\n"; exit(1);
            }
            break;
        case 2:
            switch (type) {
                case 1: { LinkedListStack<int> s; inputStack(s, "Push int"); break; }
                case 2: { LinkedListStack<float> s; inputStack(s, "Push float"); break; }
                case 3: { LinkedListStack<string> s; inputStack(s, "Push string"); break; }
                default: cout << "Invalid data type.\n"; exit(1);
            }
            break;
        default: cout << "Invalid stack type.\n"; exit(1);
    }
}

void testQueue() {
    int impl, type;
    cout << "Queue type:\n 1. Array Queue\n 2. Linked List Queue\nChoose (1-2): ";
    cin >> impl;

    cout << "Data type:\n 1. int\n 2. float\n 3. string\nChoose (1-3): ";
    cin >> type;

    switch (impl) {
        case 1:
            switch (type) {
                case 1: { ArrayQueue<int> q; inputQueue(q, "Enqueue int"); break; }
                case 2: { ArrayQueue<float> q; inputQueue(q, "Enqueue float"); break; }
                case 3: { ArrayQueue<string> q; inputQueue(q, "Enqueue string"); break; }
                default: cout << "Invalid data type.\n"; exit(1);
            }
            break;
        case 2:
            switch (type) {
                case 1: { LinkedListQueue<int> q; inputQueue(q, "Enqueue int"); break; }
                case 2: { LinkedListQueue<float> q; inputQueue(q, "Enqueue float"); break; }
                case 3: { LinkedListQueue<string> q; inputQueue(q, "Enqueue string"); break; }
                default: cout << "Invalid data type.\n"; exit(1);
            }
            break;
        default: cout << "Invalid queue type.\n"; exit(1);
    }
}

// === Entry Point ===

int main() {
    cout << "\n=== DSA Tester ===\n";
    cout << "1. Array\n2. Linked List\n3. Stack\n4. Queue\nChoose (1-4): ";

    int option;
    cin >> option;
    clearInput();

    switch (option) {
        case 1: testArray(); break;
        case 2: testLinkedList(); break;
        case 3: testStack(); break;
        case 4: testQueue(); break;
        default: cout << "Invalid top-level choice.\n"; return 1;
    }

    return 0;
}
