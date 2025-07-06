# DSAUtility

A comprehensive, header-only C++ library for Data Structures and Algorithms with modern C++17 features, template support, and extensive testing.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/std/the-standard)
[![CMake](https://img.shields.io/badge/CMake-3.16+-green.svg)](https://cmake.org/)

## 🚀 Features

### Data Structures
- **ArrayDS**: Dynamic array with template support
- **LinkedList**: Singly linked list with inheritance hierarchy
- **DoublyLinkedList**: Inherits from LinkedList
- **CircularLinkedList**: Inherits from LinkedList
- **Stack**: LIFO data structure
- **Queue**: FIFO data structure with inheritance
- **CircularQueue**: Inherits from Queue
- **Tree**: Base tree class with virtual methods
- **BinarySearchTree**: Inherits from Tree
- **AVLTree**: Self-balancing BST inheriting from Tree
- **Heap**: Binary heap with min/max variants
- **PriorityQueue**: Heap-based priority queue
- **Trie**: Generic trie with string specialization
- **Graph**: Adjacency list with traversal, shortest path, MST
- **DisjointSet**: Union-find with path compression and union by rank

### Algorithms
- **Sorting**: QuickSort, MergeSort, HeapSort, CountSort, RadixSort, ShellSort
- **Searching**: Linear, Binary, Exponential, Interpolation Search

### Utilities
- **Print**: Template printing utilities
- **Timer**: High-precision timing utilities
- **Comparator**: Flexible comparison utilities
- **Swap**: Efficient swapping utilities
- **Logger**: Comprehensive logging system

## 📋 Requirements

- **C++17** or later
- **CMake 3.16** or later
- **Compiler**: GCC 7+, Clang 6+, MSVC 2017+

## 🛠️ Installation

### Using CMake (Recommended)

```bash
# Clone the repository
git clone https://github.com/yourusername/DSAUtility.git
cd DSAUtility

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make -j$(nproc)

# Run tests
make run_tests

# Install (optional)
sudo make install
```

### Using as Header-Only Library

Simply include the umbrella header:

```cpp
#include "DSAUtility.hpp"
```

## 📖 Usage

### Basic Example

```cpp
#include "DSAUtility.hpp"
#include <iostream>
#include <vector>

using namespace DSAUtility;

int main() {
    // Create a vector to sort
    std::vector<int> numbers = {64, 34, 25, 12, 22, 11, 90};
    
    std::cout << "Original: ";
    Print::printContainer(numbers);
    
    // Sort using QuickSort
    Algorithms::QuickSort::sort(numbers);
    
    std::cout << "Sorted: ";
    Print::printContainer(numbers);
    
    // Search for an element
    auto result = Algorithms::Search::binarySearch(numbers, 25);
    if (result != numbers.end()) {
        std::cout << "Found 25 at index: " 
                  << std::distance(numbers.begin(), result) << std::endl;
    }
    
    return 0;
}
```

### Data Structure Example

```cpp
#include "DSAUtility.hpp"
#include <iostream>

using namespace DSAUtility;

int main() {
    // Create a binary search tree
    Structures::BinarySearchTree<int> bst;
    
    // Insert elements
    bst.insert(50);
    bst.insert(30);
    bst.insert(70);
    bst.insert(20);
    bst.insert(40);
    bst.insert(60);
    bst.insert(80);
    
    std::cout << "BST Inorder traversal: ";
    bst.inorderTraversal([](int value) {
        std::cout << value << " ";
    });
    std::cout << std::endl;
    
    // Search for an element
    if (bst.search(40)) {
        std::cout << "40 found in BST" << std::endl;
    }
    
    return 0;
}
```

### Custom Objects Example

```cpp
#include "DSAUtility.hpp"
#include <iostream>
#include <vector>

using namespace DSAUtility;

struct Student {
    int id;
    std::string name;
    double gpa;
    
    Student(int i, const std::string& n, double g) 
        : id(i), name(n), gpa(g) {}
    
    bool operator<(const Student& other) const {
        return id < other.id;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Student& s) {
        return os << "Student(" << s.id << ", " << s.name << ", " << s.gpa << ")";
    }
};

int main() {
    std::vector<Student> students = {
        Student(1001, "Alice", 3.8),
        Student(1002, "Bob", 3.5),
        Student(1003, "Charlie", 3.9)
    };
    
    std::cout << "Original students: ";
    Print::printContainer(students);
    
    // Sort by ID (default)
    Algorithms::QuickSort::sort(students);
    
    std::cout << "Sorted students: ";
    Print::printContainer(students);
    
    return 0;
}
```

## 🧪 Testing

The library includes comprehensive test suites for all components:

```bash
# Run all tests
make run_tests

# Run specific test
./Test_QuickSort
./Test_BinarySearchTree
./Test_Graph
```

## 📚 Documentation

### Data Structures

#### ArrayDS
```cpp
Structures::ArrayDS<int> arr;
arr.push_back(1);
arr.push_back(2);
arr.push_back(3);
```

#### LinkedList
```cpp
Structures::LinkedList<int> list;
list.insertAtEnd(1);
list.insertAtEnd(2);
list.insertAtEnd(3);
```

#### Stack
```cpp
Structures::Stack<int> stack;
stack.push(1);
stack.push(2);
int top = stack.top();
stack.pop();
```

#### Queue
```cpp
Structures::Queue<int> queue;
queue.enqueue(1);
queue.enqueue(2);
int front = queue.front();
queue.dequeue();
```

#### BinarySearchTree
```cpp
Structures::BinarySearchTree<int> bst;
bst.insert(50);
bst.insert(30);
bst.insert(70);
bool found = bst.search(30);
```

#### Graph
```cpp
Structures::Graph<int> graph;
graph.addEdge(0, 1);
graph.addEdge(0, 2);
graph.addEdge(1, 2);
auto path = graph.shortestPath(0, 2);
```

### Algorithms

#### Sorting
```cpp
std::vector<int> data = {64, 34, 25, 12, 22, 11, 90};

// QuickSort
Algorithms::QuickSort::sort(data);

// MergeSort
Algorithms::MergeSort::sort(data);

// HeapSort
Algorithms::HeapSort::sort(data);

// RadixSort
Algorithms::RadixSort::sort(data);
```

#### Searching
```cpp
std::vector<int> sorted = {1, 3, 5, 7, 9, 11, 13};

// Binary Search
auto result = Algorithms::Search::binarySearch(sorted, 7);

// Linear Search
auto result = Algorithms::Search::linearSearch(sorted, 7);

// Exponential Search
auto result = Algorithms::ExponentialSearch::search(sorted, 7);
```

### Utilities

#### Timer
```cpp
Timer timer;
timer.start();
// ... your code ...
timer.stop();
std::cout << "Time: " << timer.getElapsedTime() << " ms" << std::endl;
```

#### Logger
```cpp
Logger::info("Application started");
Logger::debug("Processing data...");
Logger::error("An error occurred");
```

## 🔧 Building from Source

### Prerequisites
- CMake 3.16+
- C++17 compatible compiler
- Make or Ninja build system

### Build Commands

```bash
# Basic build
mkdir build && cd build
cmake ..
make

# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# Release build with optimizations
cmake -DCMAKE_BUILD_TYPE=Release ..
make

# Run tests
make run_tests

# Install
sudo make install
```

### CMake Options

- `-DCMAKE_BUILD_TYPE=Debug/Release`: Set build type
- `-DCMAKE_INSTALL_PREFIX=/path/to/install`: Set install prefix
- `-DBUILD_TESTS=ON/OFF`: Enable/disable test building

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Development Setup

```bash
# Clone and setup
git clone https://github.com/yourusername/DSAUtility.git
cd DSAUtility

# Build with tests
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)

# Run tests
make run_tests
```

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Modern C++ features and best practices
- Comprehensive testing approach
- Template-based design for maximum flexibility
- Educational focus for learning DSA concepts

## 📊 Performance

The library is optimized for:
- **Educational use**: Clear, well-documented implementations
- **Competitive programming**: Fast, reliable algorithms
- **Production code**: Robust, tested data structures
- **Research**: Algorithm comparison and analysis

## 🐛 Reporting Issues

Please report bugs and request features through the [GitHub Issues](https://github.com/yourusername/DSAUtility/issues) page.

## 📞 Support

For questions and support:
- Create an issue on GitHub
- Check the documentation in the `Include/` directory
- Review the test files in `Demo/` for usage examples

---

**DSAUtility** - Making Data Structures and Algorithms accessible and efficient in modern C++.
