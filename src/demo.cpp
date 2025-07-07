#include <iostream>
#include <string>
#include <limits>
#include <algorithm>
#include <vector>

// Array
#include "structure/Linear/array/Array.hpp"
#include "structure//Linear/array/ArrayUtils.hpp"

// Linked Lists
#include "structure/Linear/list/LinkedList.hpp"
#include "structure/Linear/list/DoublyLinkedList.hpp"
#include "structure/Linear/list/CircularLinkedList.hpp"

// Stack
#include "structure/Linear/stack/IStack.hpp"
#include "structure/Linear/stack/ArrayStack.hpp"
#include "structure/Linear/stack/LinkedListStack.hpp"

// Queue
#include "structure/Linear/queue/IQueue.hpp"
#include "structure/Linear/queue/ArrayQueue.hpp"
#include "structure/Linear/queue/LinkedListQueue.hpp"

// Sorting Algorithms
#include "Algorithms/Sort.hpp"
#include "Algorithms/QuickSort.hpp"
#include "Algorithms/MergeSort.hpp"
#include "Algorithms/HeapSort.hpp"
#include "Algorithms/BinarySearch.hpp"
#include "Algorithms/BFS.hpp"
#include "Algorithms/DFS.hpp"
#include "Algorithms/Dijkstra.hpp"
#include "Algorithms/UnionFind.hpp"
#include "Algorithms/LCA.hpp"

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

void testAlgorithms() {
    cout << "\nAlgorithms Demo:\n";
    cout << "1. Sorting Algorithms\n2. Binary Search\n3. BFS\n4. DFS\n5. Dijkstra\n6. Union-Find\n7. LCA (binary tree with parent)\nChoose (1-7): ";
    int opt; cin >> opt; clearInput();
    switch (opt) {
        case 1: {
            using namespace algo;
            int algoType;
            cout << "Sorting Algorithm:\n 1. QuickSort\n 2. MergeSort\n 3. HeapSort\nChoose (1-3): ";
            cin >> algoType;
            clearInput();
            int n;
            cout << "Enter number of elements: ";
            cin >> n;
            clearInput();
            vector<int> arr(n);
            for (int i = 0; i < n; ++i) {
                cout << "Element " << (i + 1) << ": ";
                cin >> arr[i];
            }
            cout << "\nOriginal array: ";
            for (int x : arr) cout << x << ' ';
            cout << endl;
            switch (algoType) {
                case 1:
                    quickSort(arr.begin(), arr.end());
                    cout << "Sorted with QuickSort: ";
                    break;
                case 2:
                    mergeSort(arr.begin(), arr.end());
                    cout << "Sorted with MergeSort: ";
                    break;
                case 3:
                    heapSort(arr.begin(), arr.end());
                    cout << "Sorted with HeapSort: ";
                    break;
                default:
                    cout << "Invalid sorting algorithm.\n";
                    return;
            }
            for (int x : arr) cout << x << ' ';
            cout << endl;
            break;
        }
        case 2: {
            vector<int> arr = {1, 3, 5, 7, 9};
            int val; cout << "Enter value to search: "; cin >> val;
            bool found = ::binary_search(arr.begin(), arr.end(), val);
            cout << (found ? "Found\n" : "Not found\n");
            break;
        }
        case 3: {
            vector<vector<int>> adj = {{1,2},{0,3},{0,3},{1,2}};
            cout << "BFS from node 0: ";
            bfs(adj, 0, [](int u){ cout << u << ' '; });
            cout << endl;
            break;
        }
        case 4: {
            vector<vector<int>> adj = {{1,2},{0,3},{0,3},{1,2}};
            vector<bool> visited(4, false);
            cout << "DFS from node 0: ";
            dfs(adj, 0, visited, [](int u){ cout << u << ' '; });
            cout << endl;
            break;
        }
        case 5: {
            vector<vector<pair<int,int>>> adj = {
                {{1,2},{2,4}}, // 0
                {{2,1}},       // 1
                {{3,1}},       // 2
                {}             // 3
            };
            auto dist = dijkstra(adj, 0);
            cout << "Dijkstra distances from 0: ";
            for (auto d : dist) cout << d << ' ';
            cout << endl;
            break;
        }
        case 6: {
            UnionFind uf(5);
            uf.unite(0,1); uf.unite(1,2);
            cout << "0 and 2 connected? " << (uf.connected(0,2)?"Yes":"No") << endl;
            cout << "3 and 4 connected? " << (uf.connected(3,4)?"Yes":"No") << endl;
            uf.unite(3,4);
            cout << "3 and 4 connected after union? " << (uf.connected(3,4)?"Yes":"No") << endl;
            break;
        }
        case 7: {
            struct Node { int val; Node* left=nullptr; Node* right=nullptr; Node* parent=nullptr; Node(int v):val(v){} };
            Node n1(1), n2(2), n3(3), n4(4), n5(5);
            n1.left=&n2; n1.right=&n3; n2.parent=&n1; n3.parent=&n1;
            n2.left=&n4; n2.right=&n5; n4.parent=&n2; n5.parent=&n2;
            Node* res = lca(&n4, &n5);
            cout << "LCA of 4 and 5: " << (res?res->val:-1) << endl;
            break;
        }
        default: cout << "Invalid algorithm choice.\n";
    }
}

// === Entry Point ===

int main() {
    cout << "\n=== DSA Tester ===\n";
    cout << "1. Array\n2. Linked List\n3. Stack\n4. Queue\n5. Algorithms\nChoose (1-5): ";

    int option;
    cin >> option;
    clearInput();

    switch (option) {
        case 1: testArray(); break;
        case 2: testLinkedList(); break;
        case 3: testStack(); break;
        case 4: testQueue(); break;
        case 5: testAlgorithms(); break;
        default: cout << "Invalid top-level choice.\n"; return 1;
    }

    return 0;
}
