#pragma once

#include <limits>
#include <list>
#include <set>
#include <vector>

typedef size_t Vertex;
typedef std::set<Vertex> Neighbours;
typedef std::vector<Neighbours> Edges;

typedef unsigned long Distance;
typedef std::vector<std::vector<Distance>> DistancesMatrix;

class Graph {
public:
    static const Distance inf = std::numeric_limits<Distance>::max() / 2;

    Vertex vertexCount = 0;
    Edges edges;

    Graph(Vertex vertexCount = 0);
    Graph(const Edges& e);
    Graph(const Graph& o) : Graph(o.edges) {} // copy constructor

    ~Graph();

    // adding vertices and edges
    Vertex addVertex();
    Vertex addVertex(const Neighbours& neighbours); // new vertex with edges to !existing! vertices
    void addEdge(Vertex a, Vertex b);
    Vertex mergeGraph(const Graph& o); // adds other to this, without any edges between

    bool areVerticesNeighbours(Vertex a, Vertex b);

    void calculateDistancesToRootBFS(DistancesMatrix& distances, Vertex limit = 0);

    Distance calculateSumOfDistances(DistancesMatrix& distances);
    // Distance calculateSumOfDistancesBFS(); // simple BFS algorithm
    Distance calculateSumOfDistancesFW(); // Floyd–Warshall algorithm

    const void print();
};
