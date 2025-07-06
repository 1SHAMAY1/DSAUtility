#pragma once
#include <algorithm>
#include <utility>
#include <vector>
#include <array>
#include <string>
#include <type_traits>

namespace dsa {
namespace utils {

/**
 * @brief Swap utility class with various swap functions
 */
class Swap {
public:
    // Basic swap for any type
    template<typename T>
    static void swap(T& a, T& b) {
        std::swap(a, b);
    }

    // Swap using move semantics
    template<typename T>
    static void swapMove(T& a, T& b) {
        T temp = std::move(a);
        a = std::move(b);
        b = std::move(temp);
    }

    // Swap using XOR (for integers only)
    template<typename T>
    static void swapXOR(T& a, T& b) {
        static_assert(std::is_integral<T>::value, "XOR swap only works with integral types");
        if (&a != &b) {
            a ^= b;
            b ^= a;
            a ^= b;
        }
    }

    // Swap using arithmetic operations (for integers only)
    template<typename T>
    static void swapArithmetic(T& a, T& b) {
        static_assert(std::is_integral<T>::value, "Arithmetic swap only works with integral types");
        if (&a != &b) {
            a = a + b;
            b = a - b;
            a = a - b;
        }
    }

    // Swap array elements
    template<typename T, size_t N>
    static void swapArrayElements(T (&arr)[N], size_t i, size_t j) {
        if (i < N && j < N && i != j) {
            std::swap(arr[i], arr[j]);
        }
    }

    template<typename T, size_t N>
    static void swapArrayElements(std::array<T, N>& arr, size_t i, size_t j) {
        if (i < N && j < N && i != j) {
            std::swap(arr[i], arr[j]);
        }
    }

    // Swap vector elements
    template<typename T>
    static void swapVectorElements(std::vector<T>& vec, size_t i, size_t j) {
        if (i < vec.size() && j < vec.size() && i != j) {
            std::swap(vec[i], vec[j]);
        }
    }

    // Swap ranges in vectors
    template<typename T>
    static void swapVectorRanges(std::vector<T>& vec, size_t start1, size_t end1, 
                                size_t start2, size_t end2) {
        if (start1 < vec.size() && end1 <= vec.size() && 
            start2 < vec.size() && end2 <= vec.size()) {
            std::swap_ranges(vec.begin() + start1, vec.begin() + end1, 
                           vec.begin() + start2);
        }
    }

    // Swap subarrays
    template<typename T>
    static void swapSubarrays(T* arr, size_t start1, size_t end1, 
                             size_t start2, size_t end2) {
        size_t len1 = end1 - start1;
        size_t len2 = end2 - start2;
        
        if (len1 != len2) return; // Can only swap equal length subarrays
        
        for (size_t i = 0; i < len1; ++i) {
            std::swap(arr[start1 + i], arr[start2 + i]);
        }
    }

    // Swap using temporary variable (explicit)
    template<typename T>
    static void swapWithTemp(T& a, T& b) {
        T temp = a;
        a = b;
        b = temp;
    }

    // Swap using std::exchange
    template<typename T>
    static void swapWithExchange(T& a, T& b) {
        T temp = std::exchange(a, b);
        b = temp;
    }

    // Conditional swap
    template<typename T>
    static void conditionalSwap(T& a, T& b, bool condition) {
        if (condition) {
            std::swap(a, b);
        }
    }

    // Swap if greater (useful for sorting)
    template<typename T>
    static void swapIfGreater(T& a, T& b) {
        if (a > b) {
            std::swap(a, b);
        }
    }

    // Swap if less (useful for sorting)
    template<typename T>
    static void swapIfLess(T& a, T& b) {
        if (a < b) {
            std::swap(a, b);
        }
    }

    // Swap with custom comparator
    template<typename T, typename Compare>
    static void swapIf(T& a, T& b, Compare comp) {
        if (comp(a, b)) {
            std::swap(a, b);
        }
    }

    // Swap multiple values
    template<typename T>
    static void swapMultiple(T& a, T& b, T& c) {
        T temp = a;
        a = b;
        b = c;
        c = temp;
    }

    template<typename T>
    static void swapMultiple(T& a, T& b, T& c, T& d) {
        T temp = a;
        a = b;
        b = c;
        c = d;
        d = temp;
    }

    // Circular swap (rotate)
    template<typename T>
    static void circularSwap(T& a, T& b, T& c) {
        T temp = a;
        a = b;
        b = c;
        c = temp;
    }

    // Swap with bounds checking
    template<typename T>
    static bool safeSwap(T& a, T& b, size_t max_size = SIZE_MAX) {
        if (sizeof(T) <= max_size) {
            std::swap(a, b);
            return true;
        }
        return false;
    }

    // Swap pointers
    template<typename T>
    static void swapPointers(T*& a, T*& b) {
        std::swap(a, b);
    }

    // Swap smart pointers
    template<typename T>
    static void swapSmartPointers(std::unique_ptr<T>& a, std::unique_ptr<T>& b) {
        a.swap(b);
    }

    template<typename T>
    static void swapSmartPointers(std::shared_ptr<T>& a, std::shared_ptr<T>& b) {
        a.swap(b);
    }

    // Swap string characters
    static void swapStringChars(std::string& str, size_t i, size_t j) {
        if (i < str.length() && j < str.length() && i != j) {
            std::swap(str[i], str[j]);
        }
    }

    // Reverse a range (using swaps)
    template<typename Iterator>
    static void reverseRange(Iterator first, Iterator last) {
        while (first != last && first != --last) {
            std::iter_swap(first++, last);
        }
    }

    // Reverse array
    template<typename T, size_t N>
    static void reverseArray(T (&arr)[N]) {
        for (size_t i = 0; i < N / 2; ++i) {
            std::swap(arr[i], arr[N - 1 - i]);
        }
    }

    template<typename T, size_t N>
    static void reverseArray(std::array<T, N>& arr) {
        for (size_t i = 0; i < N / 2; ++i) {
            std::swap(arr[i], arr[N - 1 - i]);
        }
    }

    // Reverse vector
    template<typename T>
    static void reverseVector(std::vector<T>& vec) {
        for (size_t i = 0; i < vec.size() / 2; ++i) {
            std::swap(vec[i], vec[vec.size() - 1 - i]);
        }
    }

    // Swap with move semantics for containers
    template<typename Container>
    static void swapContainers(Container& a, Container& b) {
        a.swap(b);
    }

    // Swap using iterators
    template<typename Iterator>
    static void swapIterators(Iterator it1, Iterator it2) {
        std::iter_swap(it1, it2);
    }

    // Swap ranges using iterators
    template<typename Iterator1, typename Iterator2>
    static void swapRanges(Iterator1 first1, Iterator1 last1, Iterator2 first2) {
        std::swap_ranges(first1, last1, first2);
    }
};

// Convenience functions
template<typename T>
void swap(T& a, T& b) {
    Swap::swap(a, b);
}

template<typename T>
void swapMove(T& a, T& b) {
    Swap::swapMove(a, b);
}

template<typename T>
void swapXOR(T& a, T& b) {
    Swap::swapXOR(a, b);
}

template<typename T>
void swapIfGreater(T& a, T& b) {
    Swap::swapIfGreater(a, b);
}

template<typename T>
void swapIfLess(T& a, T& b) {
    Swap::swapIfLess(a, b);
}

} // namespace utils
} // namespace dsa
