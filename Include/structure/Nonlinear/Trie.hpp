#pragma once
#include <unordered_map>
#include <memory>
#include <string>

// Template-based Trie (default: char)
template<typename Key = char>
class Trie {
    struct Node {
        std::unordered_map<Key, std::unique_ptr<Node>> children;
        bool is_end = false;
    };
    std::unique_ptr<Node> root;
public:
    Trie() : root(std::make_unique<Node>()) {}

    // Insert a key sequence (e.g., string)
    template<typename It>
    void insert(It first, It last) {
        Node* node = root.get();
        for (auto it = first; it != last; ++it) {
            node = (node->children[*it] ? node->children[*it].get() : (node->children[*it] = std::make_unique<Node>()).get());
        }
        node->is_end = true;
    }
    void insert(const std::basic_string<Key>& word) { insert(word.begin(), word.end()); }

    // Search for a key sequence
    template<typename It>
    bool search(It first, It last) const {
        const Node* node = root.get();
        for (auto it = first; it != last; ++it) {
            auto found = node->children.find(*it);
            if (found == node->children.end()) return false;
            node = found->second.get();
        }
        return node->is_end;
    }
    bool search(const std::basic_string<Key>& word) const { return search(word.begin(), word.end()); }

    // Remove a key sequence
    template<typename It>
    bool remove(It first, It last) { return removeHelper(root, first, last); }
    bool remove(const std::basic_string<Key>& word) { return remove(word.begin(), word.end()); }
private:
    template<typename It>
    bool removeHelper(std::unique_ptr<Node>& node, It first, It last) {
        if (!node) return false;
        if (first == last) {
            if (!node->is_end) return false;
            node->is_end = false;
            return node->children.empty();
        }
        auto found = node->children.find(*first);
        if (found == node->children.end()) return false;
        bool should_delete = removeHelper(found->second, std::next(first), last);
        if (should_delete) node->children.erase(*first);
        return !node->is_end && node->children.empty();
    }
}; 