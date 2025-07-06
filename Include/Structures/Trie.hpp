#pragma once
#include <memory>
#include <stdexcept>
#include <iostream>
#include <initializer_list>
#include <vector>
#include <string>
#include <unordered_map>
#include "../Utils/Print.hpp"
#include "../Utils/Logger.hpp"

namespace dsa {
namespace structures {

/**
 * @brief Trie Node structure
 */
template<typename T>
struct TrieNode {
    std::unordered_map<T, std::unique_ptr<TrieNode<T>>> children;
    bool is_end_of_word;
    T value;
    
    TrieNode(const T& val = T{}) : is_end_of_word(false), value(val) {}
    TrieNode(T&& val) : is_end_of_word(false), value(std::move(val)) {}
};

/**
 * @brief Trie Data Structure
 */
template<typename T>
class Trie {
private:
    std::unique_ptr<TrieNode<T>> root;
    size_t size_;

public:
    // Constructors
    Trie() : root(std::make_unique<TrieNode<T>>()), size_(0) {}
    
    Trie(std::initializer_list<std::vector<T>> init_list) : root(std::make_unique<TrieNode<T>>()), size_(0) {
        for (const auto& sequence : init_list) {
            insert(sequence);
        }
    }
    
    Trie(const Trie& other) : root(std::make_unique<TrieNode<T>>()), size_(0) {
        copy_trie(other.root.get(), root.get());
    }
    
    Trie(Trie&& other) noexcept 
        : root(std::move(other.root)), size_(other.size_) {
        other.size_ = 0;
    }
    
    // Destructor
    ~Trie() = default;
    
    // Assignment operators
    Trie& operator=(const Trie& other) {
        if (this != &other) {
            clear();
            copy_trie(other.root.get(), root.get());
        }
        return *this;
    }
    
    Trie& operator=(Trie&& other) noexcept {
        if (this != &other) {
            clear();
            root = std::move(other.root);
            size_ = other.size_;
            other.size_ = 0;
        }
        return *this;
    }
    
    // Capacity
    bool empty() const { return size_ == 0; }
    size_t size() const { return size_; }
    
    // Modifiers
    void insert(const std::vector<T>& sequence) {
        TrieNode<T>* current = root.get();
        
        for (const auto& item : sequence) {
            if (current->children.find(item) == current->children.end()) {
                current->children[item] = std::make_unique<TrieNode<T>>(item);
            }
            current = current->children[item].get();
        }
        
        if (!current->is_end_of_word) {
            current->is_end_of_word = true;
            size_++;
        }
    }
    
    void insert(std::vector<T>&& sequence) {
        TrieNode<T>* current = root.get();
        
        for (auto& item : sequence) {
            if (current->children.find(item) == current->children.end()) {
                current->children[std::move(item)] = std::make_unique<TrieNode<T>>(item);
            }
            current = current->children[item].get();
        }
        
        if (!current->is_end_of_word) {
            current->is_end_of_word = true;
            size_++;
        }
    }
    
    void remove(const std::vector<T>& sequence) {
        if (contains(sequence)) {
            remove_recursive(root.get(), sequence, 0);
            size_--;
        }
    }
    
    void clear() {
        root = std::make_unique<TrieNode<T>>();
        size_ = 0;
    }
    
    // Search operations
    bool contains(const std::vector<T>& sequence) const {
        TrieNode<T>* current = root.get();
        
        for (const auto& item : sequence) {
            if (current->children.find(item) == current->children.end()) {
                return false;
            }
            current = current->children.at(item).get();
        }
        
        return current->is_end_of_word;
    }
    
    bool starts_with(const std::vector<T>& prefix) const {
        TrieNode<T>* current = root.get();
        
        for (const auto& item : prefix) {
            if (current->children.find(item) == current->children.end()) {
                return false;
            }
            current = current->children.at(item).get();
        }
        
        return true;
    }
    
    // Trie operations
    std::vector<std::vector<T>> get_all_with_prefix(const std::vector<T>& prefix) const {
        std::vector<std::vector<T>> result;
        TrieNode<T>* current = root.get();
        
        // Navigate to the prefix node
        for (const auto& item : prefix) {
            if (current->children.find(item) == current->children.end()) {
                return result; // Prefix not found
            }
            current = current->children.at(item).get();
        }
        
        // Collect all sequences starting with this prefix
        std::vector<T> current_sequence = prefix;
        collect_sequences(current, current_sequence, result);
        
        return result;
    }
    
    std::vector<std::vector<T>> get_all_sequences() const {
        std::vector<std::vector<T>> result;
        std::vector<T> current_sequence;
        collect_sequences(root.get(), current_sequence, result);
        return result;
    }
    
    size_t count_with_prefix(const std::vector<T>& prefix) const {
        TrieNode<T>* current = root.get();
        
        // Navigate to the prefix node
        for (const auto& item : prefix) {
            if (current->children.find(item) == current->children.end()) {
                return 0; // Prefix not found
            }
            current = current->children.at(item).get();
        }
        
        // Count all sequences starting with this prefix
        return count_sequences(current);
    }
    
    // Longest common prefix
    std::vector<T> longest_common_prefix() const {
        std::vector<T> result;
        TrieNode<T>* current = root.get();
        
        while (current->children.size() == 1 && !current->is_end_of_word) {
            auto it = current->children.begin();
            result.push_back(it->first);
            current = it->second.get();
        }
        
        return result;
    }
    
    // Trie properties
    size_t height() const {
        return height_recursive(root.get());
    }
    
    size_t node_count() const {
        return node_count_recursive(root.get());
    }
    
    bool is_compact() const {
        return is_compact_recursive(root.get());
    }
    
    // Utility functions
    void print() const {
        print_recursive(root.get(), "", true);
    }
    
    void print_with_prefix(const std::vector<T>& prefix) const {
        auto sequences = get_all_with_prefix(prefix);
        std::cout << "Sequences with prefix: ";
        for (const auto& seq : sequences) {
            utils::Print::printVector(seq);
            std::cout << " ";
        }
        std::cout << std::endl;
    }
    
    std::string to_string() const {
        std::stringstream ss;
        ss << "Trie[size=" << size_ << ", height=" << height() << ", nodes=" << node_count() << "]";
        return ss.str();
    }

private:
    // Helper functions
    void copy_trie(TrieNode<T>* source, TrieNode<T>* destination) {
        destination->is_end_of_word = source->is_end_of_word;
        destination->value = source->value;
        
        for (const auto& pair : source->children) {
            destination->children[pair.first] = std::make_unique<TrieNode<T>>(pair.first);
            copy_trie(pair.second.get(), destination->children[pair.first].get());
        }
    }
    
    bool remove_recursive(TrieNode<T>* node, const std::vector<T>& sequence, size_t index) {
        if (index == sequence.size()) {
            if (node->is_end_of_word) {
                node->is_end_of_word = false;
                return node->children.empty();
            }
            return false;
        }
        
        auto it = node->children.find(sequence[index]);
        if (it == node->children.end()) {
            return false;
        }
        
        bool should_delete = remove_recursive(it->second.get(), sequence, index + 1);
        
        if (should_delete) {
            node->children.erase(it);
            return !node->is_end_of_word && node->children.empty();
        }
        
        return false;
    }
    
    void collect_sequences(TrieNode<T>* node, std::vector<T>& current_sequence, 
                          std::vector<std::vector<T>>& result) const {
        if (node->is_end_of_word) {
            result.push_back(current_sequence);
        }
        
        for (const auto& pair : node->children) {
            current_sequence.push_back(pair.first);
            collect_sequences(pair.second.get(), current_sequence, result);
            current_sequence.pop_back();
        }
    }
    
    size_t count_sequences(TrieNode<T>* node) const {
        size_t count = node->is_end_of_word ? 1 : 0;
        
        for (const auto& pair : node->children) {
            count += count_sequences(pair.second.get());
        }
        
        return count;
    }
    
    size_t height_recursive(TrieNode<T>* node) const {
        if (node->children.empty()) {
            return 0;
        }
        
        size_t max_height = 0;
        for (const auto& pair : node->children) {
            max_height = std::max(max_height, height_recursive(pair.second.get()));
        }
        
        return 1 + max_height;
    }
    
    size_t node_count_recursive(TrieNode<T>* node) const {
        size_t count = 1;
        
        for (const auto& pair : node->children) {
            count += node_count_recursive(pair.second.get());
        }
        
        return count;
    }
    
    bool is_compact_recursive(TrieNode<T>* node) const {
        if (node->children.empty()) {
            return true;
        }
        
        if (node->children.size() == 1) {
            auto it = node->children.begin();
            return !node->is_end_of_word && is_compact_recursive(it->second.get());
        }
        
        for (const auto& pair : node->children) {
            if (!is_compact_recursive(pair.second.get())) {
                return false;
            }
        }
        
        return true;
    }
    
    void print_recursive(TrieNode<T>* node, const std::string& prefix, bool is_last) const {
        std::cout << prefix;
        std::cout << (is_last ? "└── " : "├── ");
        std::cout << node->value << (node->is_end_of_word ? " (*)" : "") << std::endl;
        
        size_t child_count = node->children.size();
        size_t current = 0;
        
        for (const auto& pair : node->children) {
            bool is_last_child = (current == child_count - 1);
            print_recursive(pair.second.get(), prefix + (is_last ? "    " : "│   "), is_last_child);
            current++;
        }
    }
};

// Specialization for string Trie
template<>
class Trie<char> {
private:
    std::unique_ptr<TrieNode<char>> root;
    size_t size_;

public:
    // Constructors
    Trie() : root(std::make_unique<TrieNode<char>>()), size_(0) {}
    
    Trie(std::initializer_list<std::string> init_list) : root(std::make_unique<TrieNode<char>>()), size_(0) {
        for (const auto& word : init_list) {
            insert(word);
        }
    }
    
    Trie(const Trie& other) : root(std::make_unique<TrieNode<char>>()), size_(0) {
        copy_trie(other.root.get(), root.get());
    }
    
    Trie(Trie&& other) noexcept 
        : root(std::move(other.root)), size_(other.size_) {
        other.size_ = 0;
    }
    
    // Destructor
    ~Trie() = default;
    
    // Assignment operators
    Trie& operator=(const Trie& other) {
        if (this != &other) {
            clear();
            copy_trie(other.root.get(), root.get());
        }
        return *this;
    }
    
    Trie& operator=(Trie&& other) noexcept {
        if (this != &other) {
            clear();
            root = std::move(other.root);
            size_ = other.size_;
            other.size_ = 0;
        }
        return *this;
    }
    
    // Capacity
    bool empty() const { return size_ == 0; }
    size_t size() const { return size_; }
    
    // Modifiers
    void insert(const std::string& word) {
        TrieNode<char>* current = root.get();
        
        for (char c : word) {
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = std::make_unique<TrieNode<char>>(c);
            }
            current = current->children[c].get();
        }
        
        if (!current->is_end_of_word) {
            current->is_end_of_word = true;
            size_++;
        }
    }
    
    void remove(const std::string& word) {
        if (contains(word)) {
            remove_recursive(root.get(), word, 0);
            size_--;
        }
    }
    
    void clear() {
        root = std::make_unique<TrieNode<char>>();
        size_ = 0;
    }
    
    // Search operations
    bool contains(const std::string& word) const {
        TrieNode<char>* current = root.get();
        
        for (char c : word) {
            if (current->children.find(c) == current->children.end()) {
                return false;
            }
            current = current->children.at(c).get();
        }
        
        return current->is_end_of_word;
    }
    
    bool starts_with(const std::string& prefix) const {
        TrieNode<char>* current = root.get();
        
        for (char c : prefix) {
            if (current->children.find(c) == current->children.end()) {
                return false;
            }
            current = current->children.at(c).get();
        }
        
        return true;
    }
    
    // String-specific operations
    std::vector<std::string> get_all_with_prefix(const std::string& prefix) const {
        std::vector<std::string> result;
        TrieNode<char>* current = root.get();
        
        // Navigate to the prefix node
        for (char c : prefix) {
            if (current->children.find(c) == current->children.end()) {
                return result; // Prefix not found
            }
            current = current->children.at(c).get();
        }
        
        // Collect all words starting with this prefix
        std::string current_word = prefix;
        collect_words(current, current_word, result);
        
        return result;
    }
    
    std::vector<std::string> get_all_words() const {
        std::vector<std::string> result;
        std::string current_word;
        collect_words(root.get(), current_word, result);
        return result;
    }
    
    size_t count_with_prefix(const std::string& prefix) const {
        TrieNode<char>* current = root.get();
        
        // Navigate to the prefix node
        for (char c : prefix) {
            if (current->children.find(c) == current->children.end()) {
                return 0; // Prefix not found
            }
            current = current->children.at(c).get();
        }
        
        // Count all words starting with this prefix
        return count_words(current);
    }
    
    std::string longest_common_prefix() const {
        std::string result;
        TrieNode<char>* current = root.get();
        
        while (current->children.size() == 1 && !current->is_end_of_word) {
            auto it = current->children.begin();
            result += it->first;
            current = it->second.get();
        }
        
        return result;
    }
    
    // Utility functions
    void print() const {
        print_recursive(root.get(), "", true);
    }
    
    void print_with_prefix(const std::string& prefix) const {
        auto words = get_all_with_prefix(prefix);
        std::cout << "Words with prefix '" << prefix << "': ";
        for (const auto& word : words) {
            std::cout << word << " ";
        }
        std::cout << std::endl;
    }
    
    std::string to_string() const {
        std::stringstream ss;
        ss << "StringTrie[size=" << size_ << ", height=" << height() << "]";
        return ss.str();
    }

private:
    // Helper functions for string Trie
    void copy_trie(TrieNode<char>* source, TrieNode<char>* destination) {
        destination->is_end_of_word = source->is_end_of_word;
        destination->value = source->value;
        
        for (const auto& pair : source->children) {
            destination->children[pair.first] = std::make_unique<TrieNode<char>>(pair.first);
            copy_trie(pair.second.get(), destination->children[pair.first].get());
        }
    }
    
    bool remove_recursive(TrieNode<char>* node, const std::string& word, size_t index) {
        if (index == word.length()) {
            if (node->is_end_of_word) {
                node->is_end_of_word = false;
                return node->children.empty();
            }
            return false;
        }
        
        auto it = node->children.find(word[index]);
        if (it == node->children.end()) {
            return false;
        }
        
        bool should_delete = remove_recursive(it->second.get(), word, index + 1);
        
        if (should_delete) {
            node->children.erase(it);
            return !node->is_end_of_word && node->children.empty();
        }
        
        return false;
    }
    
    void collect_words(TrieNode<char>* node, std::string& current_word, 
                      std::vector<std::string>& result) const {
        if (node->is_end_of_word) {
            result.push_back(current_word);
        }
        
        for (const auto& pair : node->children) {
            current_word += pair.first;
            collect_words(pair.second.get(), current_word, result);
            current_word.pop_back();
        }
    }
    
    size_t count_words(TrieNode<char>* node) const {
        size_t count = node->is_end_of_word ? 1 : 0;
        
        for (const auto& pair : node->children) {
            count += count_words(pair.second.get());
        }
        
        return count;
    }
    
    size_t height() const {
        return height_recursive(root.get());
    }
    
    size_t height_recursive(TrieNode<char>* node) const {
        if (node->children.empty()) {
            return 0;
        }
        
        size_t max_height = 0;
        for (const auto& pair : node->children) {
            max_height = std::max(max_height, height_recursive(pair.second.get()));
        }
        
        return 1 + max_height;
    }
    
    void print_recursive(TrieNode<char>* node, const std::string& prefix, bool is_last) const {
        std::cout << prefix;
        std::cout << (is_last ? "└── " : "├── ");
        std::cout << (node->value ? node->value : ' ') << (node->is_end_of_word ? " (*)" : "") << std::endl;
        
        size_t child_count = node->children.size();
        size_t current = 0;
        
        for (const auto& pair : node->children) {
            bool is_last_child = (current == child_count - 1);
            print_recursive(pair.second.get(), prefix + (is_last ? "    " : "│   "), is_last_child);
            current++;
        }
    }
};

// Type aliases for common use cases
using StringTrie = Trie<char>;
using IntTrie = Trie<int>;
using CharTrie = Trie<char>;

} // namespace structures
} // namespace dsa
