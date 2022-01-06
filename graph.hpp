#pragma once

#include <vector>


class Graph {
protected:
    size_t vertexCount = 0;

public:
    virtual ~Graph() {};

    size_t len() const { return vertexCount; };

    size_t addVertex() { return vertexCount++; };
    virtual size_t addVertex(const std::vector<size_t>& neighbours) = 0;

    virtual void addEdge(size_t a, size_t b) = 0;

    virtual bool areVerticesNeighbours(size_t a, size_t b) const = 0;

    virtual unsigned int calculateBFS() const = 0;
    virtual unsigned int calculateFW() const = 0;

    virtual void printEdges() const = 0;
};
