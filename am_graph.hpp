#pragma once

#include <vector>

#include "graph.hpp"


class AMGraph : public Graph {
    typedef std::vector<bool> Neighbours;
    typedef std::vector<Neighbours> AdjacencyMatrix;

public:
    AdjacencyMatrix edges;

    AMGraph(Vertex maxVertexCount);
    AMGraph(const AMGraph& other, Vertex maxVertexCount = 0);

    ~AMGraph() {};

    using Graph::addVertex;
    Vertex addVertex(const Neighbours& neighbours);

    void addEdge(Vertex a, Vertex b) override;

    bool areVerticesNeighbours(Vertex a, Vertex b) const override;

    Vertex merge(const AMGraph& other);

    // calculate shortest distances between vertex s and every other vertex between min and max using BFS algorithm; max == 0 means max == vertexCount
    void calculateShortestDistancesFromVertexBFS(DistancesMatrix& distances, Vertex s = 0, Vertex min = 0, Vertex max = 0) const override;

    // calculate the sum of shortest distances in graph using BFS algorithm
    Distance getSumOfShortestDistancesBFS() const override;

    // prints out the graph to stdout, as adjacency lists
    const void printAdjacencyList() const override;

    // print graph as a list of vertices and a list of edges, for use with https://csacademy.com/app/graph_editor/
    const void printGraph() const override;
};
