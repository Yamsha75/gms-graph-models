#pragma once

#include <list>
#include <map>
#include <set>
#include <limits>


typedef unsigned long Vertex;

class Graph {
private:
    Vertex vertexCount = 0;
    std::map<Vertex, std::set<Vertex>> edges;

public:
    static const long inf = std::numeric_limits<long>::max() / 2;

    Graph();
    Graph(const Graph& other);
    Graph(const std::list<std::set<Vertex>> vertices);

    ~Graph();

    unsigned long getVertexCount();
    const std::map<Vertex, std::set<Vertex>> getEdges();

    Vertex addVertex();
    Vertex addVertex(const std::set<Vertex> vertexEdges);
    bool addEdge(Vertex a, Vertex b);

    Vertex mergeGraph(const Graph other);

    bool areVerticesNeighbours(Vertex a, Vertex b);

    unsigned long calculateSumOfDistances();

    void print();
};
