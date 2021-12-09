#pragma once

#include <limits>
#include <list>
#include <map>
#include <set>

typedef unsigned long vertex;
typedef unsigned long distance;

class Graph {
public:
    static const distance inf = std::numeric_limits<distance>::max() / 2;

    vertex vertexCount = 0;
    std::map<vertex, std::set<vertex>> edges;

    Graph();
    Graph(const Graph& o); // copy constructor
    Graph(const std::list<std::set<vertex>>& listOfVertexEdges);

    ~Graph();

    // adding vertices and edges
    vertex addVertex();
    vertex addVertex(const std::set<vertex>& vertexEdges); // new vertex with edges to !existing! vertices
    void addEdge(vertex a, vertex b);
    vertex mergeGraph(const Graph& o); // adds other to this, without any edges between

    bool areVerticesNeighbours(vertex a, vertex b);

    void calculateDistancesToRootBFS(std::map<vertex, std::map<vertex, distance>>& distances, vertex limit = 0);

    void calculateAllDistancesFW(std::map<vertex, std::map<vertex, distance>>& distances, bool skipInit = false);

    distance calculateSumOfDistances(std::map<vertex, std::map<vertex, distance>>& distances);

    const void print();
};
