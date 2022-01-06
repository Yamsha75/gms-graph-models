#pragma once

#include <list>

#include "graph.hpp"


class ListGraph : public Graph {
    typedef std::list<size_t> Neighbours;

private:
    Neighbours* edges;

public:
    ListGraph(size_t size) : edges(new Neighbours[size]) {};
    ListGraph(const ListGraph& other);

    ~ListGraph() { delete[] edges; };

    using Graph::addVertex;
    size_t addVertex(const std::vector<size_t>& neighbours) override;

    void addEdge(size_t a, size_t b) override;

    bool areVerticesNeighbours(size_t a, size_t b) const override;

    const Neighbours getVertexNeighbours(size_t v) const;

    size_t merge(const ListGraph& other);

    unsigned int calculateBFS() const override;
    unsigned int calculateFW() const override;

    void printEdges() const override;
};
