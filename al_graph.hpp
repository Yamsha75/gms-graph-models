#pragma once

#include <set>
#include <vector>

#include "graph.hpp"


class ALGraph : public Graph {
    typedef std::set<Vertex> Neighbours;
    typedef std::vector<Neighbours> AdjacencyLists;

public:
    AdjacencyLists edges;

    ALGraph(Vertex maxVertexCount);
    ALGraph(const ALGraph& other, Vertex maxVertexCount = 0);

    ~ALGraph() {};

    using Graph::addVertex;
    Vertex addVertex(const Neighbours& neighbours);

    void addEdge(Vertex a, Vertex b) override;

    bool areVerticesNeighbours(Vertex a, Vertex b) const override;

    Vertex merge(const ALGraph& other);

    // calculate shortest distances between vertex s and every other vertex between min and max using BFS algorithm; max == 0 means max == vertexCount
    void calculateShortestDistancesFromVertexBFS(DistancesMatrix& distances, Vertex s = 0, Vertex min = 0, Vertex max = 0) const override;

    // calculate the sum of shortest distances in graph using BFS algorithm
    Distance getSumOfShortestDistancesBFS() const override;

    // prints out the graph to stdout, as adjacency lists
    const void printAdjacencyList() const override;

    // print graph as a list of vertices and a list of edges, for use with https://csacademy.com/app/graph_editor/
    const void printGraph() const override;
};
