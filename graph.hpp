#pragma once

#include <vector>


class Graph {
private:
    size_t size;
    size_t vertexCount = 0;
    bool** edges;

public:
    Graph(size_t size);
    Graph(const Graph& other);

    ~Graph();

    size_t len() const { return vertexCount; };

    size_t addVertex();
    size_t addVertex(const std::vector<size_t>& neighbours);

    void addEdge(size_t a, size_t b);

    bool areVerticesNeighbours(size_t a, size_t b) const;

    size_t merge(const Graph& other);

    unsigned int calculateBFS() const;
    unsigned int calculateFW() const;

    void print() const;
};
