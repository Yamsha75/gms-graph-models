#pragma once

#include <list>
#include <vector>


class Graph {
    typedef std::list<size_t> Neighbours;

private:
    size_t vertexCount = 0;
    Neighbours* edges;

public:
    Graph(size_t size) : edges(new Neighbours[size]) {};
    Graph(const Graph& other);

    ~Graph() { delete[] edges; };

    size_t len() const { return vertexCount; };

    size_t addVertex();
    size_t addVertex(const std::vector<size_t>& neighbours);

    void addEdge(size_t a, size_t b);

    bool areVerticesNeighbours(size_t a, size_t b) const;
    const Neighbours getVertexNeighbours(size_t v) const;

    size_t merge(const Graph& other);

    unsigned int calculateBFS() const;
    unsigned int calculateFW() const;

    void print() const;
};
