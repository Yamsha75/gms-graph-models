#pragma once

#include "graph.hpp"


class MatrixGraph : public Graph {
private:
    const size_t size;
    bool** edges;

public:
    MatrixGraph(size_t size);
    MatrixGraph(const MatrixGraph& other);

    ~MatrixGraph();

    using Graph::addVertex;
    size_t addVertex(const std::vector<size_t>& neighbours) override;

    void addEdge(size_t a, size_t b) override;

    bool areVerticesNeighbours(size_t a, size_t b) const override;

    size_t merge(const MatrixGraph& other);

    unsigned int calculateBFS() const override;
    unsigned int calculateFW() const override;

    void printEdges() const override;
};
