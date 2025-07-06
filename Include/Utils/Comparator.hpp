#pragma once
#include <functional>
#include <string>
#include <vector>
#include <algorithm>
#include <type_traits>

namespace dsa {
namespace utils {

/**
 * @brief Comparator utility class with various comparison functions
 */
class Comparator {
public:
    // Standard comparators
    template<typename T>
    static bool less(const T& a, const T& b) {
        return a < b;
    }

    template<typename T>
    static bool greater(const T& a, const T& b) {
        return a > b;
    }

    template<typename T>
    static bool lessEqual(const T& a, const T& b) {
        return a <= b;
    }

    template<typename T>
    static bool greaterEqual(const T& a, const T& b) {
        return a >= b;
    }

    template<typename T>
    static bool equal(const T& a, const T& b) {
        return a == b;
    }

    template<typename T>
    static bool notEqual(const T& a, const T& b) {
        return a != b;
    }

    // String comparators
    static bool stringLess(const std::string& a, const std::string& b) {
        return a < b;
    }

    static bool stringGreater(const std::string& a, const std::string& b) {
        return a > b;
    }

    static bool stringLessIgnoreCase(const std::string& a, const std::string& b) {
        std::string a_lower = a;
        std::string b_lower = b;
        std::transform(a_lower.begin(), a_lower.end(), a_lower.begin(), ::tolower);
        std::transform(b_lower.begin(), b_lower.end(), b_lower.begin(), ::tolower);
        return a_lower < b_lower;
    }

    // Numeric comparators
    template<typename T>
    static bool isPositive(const T& value) {
        return value > 0;
    }

    template<typename T>
    static bool isNegative(const T& value) {
        return value < 0;
    }

    template<typename T>
    static bool isZero(const T& value) {
        return value == 0;
    }

    template<typename T>
    static bool isEven(const T& value) {
        return value % 2 == 0;
    }

    template<typename T>
    static bool isOdd(const T& value) {
        return value % 2 != 0;
    }

    // Array/Vector comparators
    template<typename T>
    static bool vectorEqual(const std::vector<T>& a, const std::vector<T>& b) {
        if (a.size() != b.size()) return false;
        for (size_t i = 0; i < a.size(); ++i) {
            if (a[i] != b[i]) return false;
        }
        return true;
    }

    template<typename T>
    static bool vectorLess(const std::vector<T>& a, const std::vector<T>& b) {
        return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    // Custom comparators
    template<typename T, typename Compare>
    static bool customCompare(const T& a, const T& b, Compare comp) {
        return comp(a, b);
    }

    // Comparator for pairs (useful for sorting by second element)
    template<typename T1, typename T2>
    static bool pairSecondLess(const std::pair<T1, T2>& a, const std::pair<T1, T2>& b) {
        return a.second < b.second;
    }

    template<typename T1, typename T2>
    static bool pairSecondGreater(const std::pair<T1, T2>& a, const std::pair<T1, T2>& b) {
        return a.second > b.second;
    }

    // Comparator for structs/classes with specific member
    template<typename T, typename MemberType>
    static bool memberLess(const T& a, const T& b, MemberType T::*member) {
        return a.*member < b.*member;
    }

    template<typename T, typename MemberType>
    static bool memberGreater(const T& a, const T& b, MemberType T::*member) {
        return a.*member > b.*member;
    }

    // Range comparators
    template<typename Iterator>
    static bool rangeEqual(Iterator first1, Iterator last1, Iterator first2) {
        return std::equal(first1, last1, first2);
    }

    template<typename Iterator>
    static bool rangeLess(Iterator first1, Iterator last1, Iterator first2, Iterator last2) {
        return std::lexicographical_compare(first1, last1, first2, last2);
    }

    // Null pointer comparators
    template<typename T>
    static bool isNull(const T* ptr) {
        return ptr == nullptr;
    }

    template<typename T>
    static bool isNotNull(const T* ptr) {
        return ptr != nullptr;
    }

    // Type-safe comparators
    template<typename T>
    static bool isNull(const std::shared_ptr<T>& ptr) {
        return ptr == nullptr;
    }

    template<typename T>
    static bool isNotNull(const std::shared_ptr<T>& ptr) {
        return ptr != nullptr;
    }

    // Floating point comparators (with epsilon)
    static bool floatEqual(double a, double b, double epsilon = 1e-9) {
        return std::abs(a - b) < epsilon;
    }

    static bool floatLess(double a, double b, double epsilon = 1e-9) {
        return a < b - epsilon;
    }

    static bool floatGreater(double a, double b, double epsilon = 1e-9) {
        return a > b + epsilon;
    }

    // Bitwise comparators
    template<typename T>
    static bool bitwiseEqual(const T& a, const T& b) {
        return !(a ^ b);
    }

    template<typename T>
    static bool hasBitSet(const T& value, int bit_position) {
        return (value & (1 << bit_position)) != 0;
    }

    // String-specific comparators
    static bool startsWith(const std::string& str, const std::string& prefix) {
        if (prefix.length() > str.length()) return false;
        return str.compare(0, prefix.length(), prefix) == 0;
    }

    static bool endsWith(const std::string& str, const std::string& suffix) {
        if (suffix.length() > str.length()) return false;
        return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
    }

    static bool contains(const std::string& str, const std::string& substr) {
        return str.find(substr) != std::string::npos;
    }

    // Case-insensitive string comparison
    static bool stringEqualIgnoreCase(const std::string& a, const std::string& b) {
        if (a.length() != b.length()) return false;
        for (size_t i = 0; i < a.length(); ++i) {
            if (tolower(a[i]) != tolower(b[i])) return false;
        }
        return true;
    }
};

// Functor classes for use with STL algorithms
template<typename T>
struct Less {
    bool operator()(const T& a, const T& b) const {
        return Comparator::less(a, b);
    }
};

template<typename T>
struct Greater {
    bool operator()(const T& a, const T& b) const {
        return Comparator::greater(a, b);
    }
};

template<typename T>
struct Equal {
    bool operator()(const T& a, const T& b) const {
        return Comparator::equal(a, b);
    }
};

// Custom comparator for case-insensitive string comparison
struct StringLessIgnoreCase {
    bool operator()(const std::string& a, const std::string& b) const {
        return Comparator::stringLessIgnoreCase(a, b);
    }
};

// Custom comparator for floating point comparison
struct FloatLess {
    double epsilon;
    FloatLess(double eps = 1e-9) : epsilon(eps) {}
    
    bool operator()(double a, double b) const {
        return Comparator::floatLess(a, b, epsilon);
    }
};

} // namespace utils
} // namespace dsa
