#pragma once
#include <memory>
#include <stdexcept>
#include <iostream>
#include <initializer_list>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <algorithm>
#include "../Utils/Print.hpp"
#include "../Utils/Logger.hpp"

namespace dsa {
namespace structures {

/**
 * @brief Graph Edge structure
 */
template<typename T>
struct Edge {
    T source;
    T destination;
    double weight;
    
    Edge(const T& src, const T& dest, double w = 1.0) 
        : source(src), destination(dest), weight(w) {}
    
    bool operator==(const Edge& other) const {
        return source == other.source && destination == other.destination;
    }
};

/**
 * @brief Graph Data Structure
 */
template<typename T>
class Graph {
private:
    std::unordered_map<T, std::vector<Edge<T>>> adjacency_list;
    bool is_directed_;
    size_t edge_count_;

public:
    // Constructors
    Graph(bool directed = false) : is_directed_(directed), edge_count_(0) {}
    
    Graph(std::initializer_list<Edge<T>> init_list, bool directed = false) 
        : is_directed_(directed), edge_count_(0) {
        for (const auto& edge : init_list) {
            add_edge(edge.source, edge.destination, edge.weight);
        }
    }
    
    Graph(const Graph& other) : adjacency_list(other.adjacency_list), 
                               is_directed_(other.is_directed_), edge_count_(other.edge_count_) {}
    
    Graph(Graph&& other) noexcept 
        : adjacency_list(std::move(other.adjacency_list)), 
          is_directed_(other.is_directed_), edge_count_(other.edge_count_) {
        other.edge_count_ = 0;
    }
    
    // Destructor
    ~Graph() = default;
    
    // Assignment operators
    Graph& operator=(const Graph& other) {
        if (this != &other) {
            adjacency_list = other.adjacency_list;
            is_directed_ = other.is_directed_;
            edge_count_ = other.edge_count_;
        }
        return *this;
    }
    
    Graph& operator=(Graph&& other) noexcept {
        if (this != &other) {
            adjacency_list = std::move(other.adjacency_list);
            is_directed_ = other.is_directed_;
            edge_count_ = other.edge_count_;
            other.edge_count_ = 0;
        }
        return *this;
    }
    
    // Capacity
    bool empty() const { return adjacency_list.empty(); }
    size_t vertex_count() const { return adjacency_list.size(); }
    size_t edge_count() const { return edge_count_; }
    bool is_directed() const { return is_directed_; }
    
    // Modifiers
    void add_vertex(const T& vertex) {
        if (adjacency_list.find(vertex) == adjacency_list.end()) {
            adjacency_list[vertex] = std::vector<Edge<T>>();
        }
    }
    
    void add_edge(const T& source, const T& destination, double weight = 1.0) {
        add_vertex(source);
        add_vertex(destination);
        
        // Add edge from source to destination
        adjacency_list[source].emplace_back(source, destination, weight);
        
        // If undirected, add edge from destination to source
        if (!is_directed_) {
            adjacency_list[destination].emplace_back(destination, source, weight);
        }
        
        edge_count_++;
    }
    
    void remove_vertex(const T& vertex) {
        if (adjacency_list.find(vertex) == adjacency_list.end()) {
            return;
        }
        
        // Remove all edges to this vertex
        for (auto& pair : adjacency_list) {
            auto& edges = pair.second;
            edges.erase(
                std::remove_if(edges.begin(), edges.end(),
                    [&vertex](const Edge<T>& edge) {
                        return edge.destination == vertex;
                    }),
                edges.end()
            );
        }
        
        // Remove the vertex
        adjacency_list.erase(vertex);
    }
    
    void remove_edge(const T& source, const T& destination) {
        if (adjacency_list.find(source) == adjacency_list.end()) {
            return;
        }
        
        auto& edges = adjacency_list[source];
        size_t initial_size = edges.size();
        
        edges.erase(
            std::remove_if(edges.begin(), edges.end(),
                [&destination](const Edge<T>& edge) {
                    return edge.destination == destination;
                }),
            edges.end()
        );
        
        if (edges.size() < initial_size) {
            edge_count_--;
        }
        
        // If undirected, also remove the reverse edge
        if (!is_directed_ && adjacency_list.find(destination) != adjacency_list.end()) {
            auto& dest_edges = adjacency_list[destination];
            initial_size = dest_edges.size();
            
            dest_edges.erase(
                std::remove_if(dest_edges.begin(), dest_edges.end(),
                    [&source](const Edge<T>& edge) {
                        return edge.destination == source;
                    }),
                dest_edges.end()
            );
            
            if (dest_edges.size() < initial_size) {
                edge_count_--;
            }
        }
    }
    
    void clear() {
        adjacency_list.clear();
        edge_count_ = 0;
    }
    
    // Graph operations
    std::vector<T> get_vertices() const {
        std::vector<T> vertices;
        for (const auto& pair : adjacency_list) {
            vertices.push_back(pair.first);
        }
        return vertices;
    }
    
    std::vector<Edge<T>> get_edges() const {
        std::vector<Edge<T>> edges;
        for (const auto& pair : adjacency_list) {
            for (const auto& edge : pair.second) {
                edges.push_back(edge);
            }
        }
        return edges;
    }
    
    std::vector<T> get_neighbors(const T& vertex) const {
        std::vector<T> neighbors;
        auto it = adjacency_list.find(vertex);
        if (it != adjacency_list.end()) {
            for (const auto& edge : it->second) {
                neighbors.push_back(edge.destination);
            }
        }
        return neighbors;
    }
    
    std::vector<Edge<T>> get_edges_from(const T& vertex) const {
        auto it = adjacency_list.find(vertex);
        if (it != adjacency_list.end()) {
            return it->second;
        }
        return std::vector<Edge<T>>();
    }
    
    bool has_vertex(const T& vertex) const {
        return adjacency_list.find(vertex) != adjacency_list.end();
    }
    
    bool has_edge(const T& source, const T& destination) const {
        auto it = adjacency_list.find(source);
        if (it == adjacency_list.end()) {
            return false;
        }
        
        for (const auto& edge : it->second) {
            if (edge.destination == destination) {
                return true;
            }
        }
        return false;
    }
    
    double get_edge_weight(const T& source, const T& destination) const {
        auto it = adjacency_list.find(source);
        if (it == adjacency_list.end()) {
            throw std::out_of_range("Source vertex not found");
        }
        
        for (const auto& edge : it->second) {
            if (edge.destination == destination) {
                return edge.weight;
            }
        }
        
        throw std::out_of_range("Edge not found");
    }
    
    // Graph traversal
    std::vector<T> breadth_first_search(const T& start_vertex) const {
        std::vector<T> result;
        std::unordered_set<T> visited;
        std::queue<T> queue;
        
        if (!has_vertex(start_vertex)) {
            return result;
        }
        
        queue.push(start_vertex);
        visited.insert(start_vertex);
        
        while (!queue.empty()) {
            T current = queue.front();
            queue.pop();
            result.push_back(current);
            
            for (const auto& neighbor : get_neighbors(current)) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    queue.push(neighbor);
                }
            }
        }
        
        return result;
    }
    
    std::vector<T> depth_first_search(const T& start_vertex) const {
        std::vector<T> result;
        std::unordered_set<T> visited;
        std::stack<T> stack;
        
        if (!has_vertex(start_vertex)) {
            return result;
        }
        
        stack.push(start_vertex);
        
        while (!stack.empty()) {
            T current = stack.top();
            stack.pop();
            
            if (visited.find(current) == visited.end()) {
                visited.insert(current);
                result.push_back(current);
                
                auto neighbors = get_neighbors(current);
                for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it) {
                    if (visited.find(*it) == visited.end()) {
                        stack.push(*it);
                    }
                }
            }
        }
        
        return result;
    }
    
    // Graph properties
    bool is_connected() const {
        if (empty()) return true;
        
        auto vertices = get_vertices();
        if (vertices.empty()) return true;
        
        auto bfs_result = breadth_first_search(vertices[0]);
        return bfs_result.size() == vertices.size();
    }
    
    bool has_cycle() const {
        std::unordered_set<T> visited;
        std::unordered_set<T> rec_stack;
        
        for (const auto& vertex : get_vertices()) {
            if (visited.find(vertex) == visited.end()) {
                if (has_cycle_dfs(vertex, visited, rec_stack)) {
                    return true;
                }
            }
        }
        return false;
    }
    
    bool is_bipartite() const {
        std::unordered_map<T, int> colors;
        
        for (const auto& vertex : get_vertices()) {
            if (colors.find(vertex) == colors.end()) {
                if (!is_bipartite_dfs(vertex, 0, colors)) {
                    return false;
                }
            }
        }
        return true;
    }
    
    // Shortest path algorithms
    std::vector<T> shortest_path(const T& source, const T& destination) const {
        std::unordered_map<T, double> distances;
        std::unordered_map<T, T> previous;
        std::unordered_set<T> unvisited;
        
        // Initialize distances
        for (const auto& vertex : get_vertices()) {
            distances[vertex] = std::numeric_limits<double>::infinity();
            unvisited.insert(vertex);
        }
        distances[source] = 0;
        
        while (!unvisited.empty()) {
            // Find vertex with minimum distance
            T current = *unvisited.begin();
            double min_dist = std::numeric_limits<double>::infinity();
            
            for (const auto& vertex : unvisited) {
                if (distances[vertex] < min_dist) {
                    min_dist = distances[vertex];
                    current = vertex;
                }
            }
            
            if (min_dist == std::numeric_limits<double>::infinity()) {
                break; // No path exists
            }
            
            unvisited.erase(current);
            
            // Update distances to neighbors
            for (const auto& edge : get_edges_from(current)) {
                T neighbor = edge.destination;
                if (unvisited.find(neighbor) != unvisited.end()) {
                    double new_dist = distances[current] + edge.weight;
                    if (new_dist < distances[neighbor]) {
                        distances[neighbor] = new_dist;
                        previous[neighbor] = current;
                    }
                }
            }
        }
        
        // Reconstruct path
        std::vector<T> path;
        T current = destination;
        
        while (previous.find(current) != previous.end()) {
            path.push_back(current);
            current = previous[current];
        }
        
        if (current == source) {
            path.push_back(source);
            std::reverse(path.begin(), path.end());
            return path;
        }
        
        return std::vector<T>(); // No path exists
    }
    
    // Minimum spanning tree (for undirected graphs)
    Graph<T> minimum_spanning_tree() const {
        if (is_directed_) {
            throw std::runtime_error("Minimum spanning tree not applicable to directed graphs");
        }
        
        Graph<T> mst(false);
        std::vector<Edge<T>> edges = get_edges();
        
        // Sort edges by weight
        std::sort(edges.begin(), edges.end(),
            [](const Edge<T>& a, const Edge<T>& b) {
                return a.weight < b.weight;
            });
        
        // Kruskal's algorithm
        std::unordered_map<T, T> parent;
        std::unordered_map<T, int> rank;
        
        // Initialize disjoint sets
        for (const auto& vertex : get_vertices()) {
            parent[vertex] = vertex;
            rank[vertex] = 0;
        }
        
        for (const auto& edge : edges) {
            T root1 = find_set(edge.source, parent);
            T root2 = find_set(edge.destination, parent);
            
            if (root1 != root2) {
                mst.add_edge(edge.source, edge.destination, edge.weight);
                union_sets(root1, root2, parent, rank);
            }
        }
        
        return mst;
    }
    
    // Utility functions
    void print() const {
        std::cout << "Graph (" << (is_directed_ ? "Directed" : "Undirected") << "):" << std::endl;
        for (const auto& pair : adjacency_list) {
            std::cout << "  " << pair.first << " -> ";
            for (size_t i = 0; i < pair.second.size(); ++i) {
                const auto& edge = pair.second[i];
                std::cout << edge.destination << "(" << edge.weight << ")";
                if (i < pair.second.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << std::endl;
        }
    }
    
    void print_matrix() const {
        auto vertices = get_vertices();
        size_t n = vertices.size();
        
        std::cout << "Adjacency Matrix:" << std::endl;
        std::cout << "    ";
        for (const auto& vertex : vertices) {
            std::cout << vertex << " ";
        }
        std::cout << std::endl;
        
        for (const auto& vertex1 : vertices) {
            std::cout << vertex1 << " ";
            for (const auto& vertex2 : vertices) {
                if (has_edge(vertex1, vertex2)) {
                    std::cout << get_edge_weight(vertex1, vertex2) << " ";
                } else {
                    std::cout << "0 ";
                }
            }
            std::cout << std::endl;
        }
    }
    
    std::string to_string() const {
        std::stringstream ss;
        ss << "Graph[vertices=" << vertex_count() << ", edges=" << edge_count() 
           << ", directed=" << (is_directed_ ? "true" : "false") << "]";
        return ss.str();
    }

private:
    // Helper functions
    bool has_cycle_dfs(const T& vertex, std::unordered_set<T>& visited, 
                       std::unordered_set<T>& rec_stack) const {
        visited.insert(vertex);
        rec_stack.insert(vertex);
        
        for (const auto& neighbor : get_neighbors(vertex)) {
            if (visited.find(neighbor) == visited.end()) {
                if (has_cycle_dfs(neighbor, visited, rec_stack)) {
                    return true;
                }
            } else if (rec_stack.find(neighbor) != rec_stack.end()) {
                return true;
            }
        }
        
        rec_stack.erase(vertex);
        return false;
    }
    
    bool is_bipartite_dfs(const T& vertex, int color, 
                          std::unordered_map<T, int>& colors) const {
        colors[vertex] = color;
        
        for (const auto& neighbor : get_neighbors(vertex)) {
            if (colors.find(neighbor) == colors.end()) {
                if (!is_bipartite_dfs(neighbor, 1 - color, colors)) {
                    return false;
                }
            } else if (colors[neighbor] == color) {
                return false;
            }
        }
        
        return true;
    }
    
    T find_set(const T& vertex, std::unordered_map<T, T>& parent) const {
        if (parent[vertex] != vertex) {
            parent[vertex] = find_set(parent[vertex], parent);
        }
        return parent[vertex];
    }
    
    void union_sets(const T& x, const T& y, std::unordered_map<T, T>& parent, 
                    std::unordered_map<T, int>& rank) const {
        T root_x = find_set(x, parent);
        T root_y = find_set(y, parent);
        
        if (root_x == root_y) return;
        
        if (rank[root_x] < rank[root_y]) {
            parent[root_x] = root_y;
        } else if (rank[root_x] > rank[root_y]) {
            parent[root_y] = root_x;
        } else {
            parent[root_y] = root_x;
            rank[root_x]++;
        }
    }
};

// Type aliases for common use cases
using IntGraph = Graph<int>;
using StringGraph = Graph<std::string>;
using CharGraph = Graph<char>;

} // namespace structures
} // namespace dsa
