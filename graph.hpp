#pragma once

#include <limits>
#include <vector>


typedef unsigned int Vertex;
typedef unsigned int Distance;

typedef std::vector<std::vector<Distance>> DistancesMatrix;


class Graph {
public:
    Vertex maxVertexCount;
    Vertex vertexCount = 0;

    static const Distance inf = std::numeric_limits<Distance>::max() / 2;

    Graph(Vertex maxVertexCount) : maxVertexCount(maxVertexCount) {};

    ~Graph() {};

    Vertex addVertex();

    virtual void addEdge(Vertex a, Vertex b) = 0;

    virtual bool areVerticesNeighbours(Vertex a, Vertex b) const = 0;

    // calculate shortest distances between vertex s and every other vertex between min and max using BFS algorithm; max == 0 means max == vertexCount
    virtual void calculateShortestDistancesFromVertexBFS(DistancesMatrix& distances, Vertex s = 0, Vertex min = 0, Vertex max = 0) const = 0;

    // calculate shortest distances between every pair of vertices between min and max using BFS algorithm; max == 0 means max == vertexCount
    void calculateShortestDistancesBetweenVerticesBFS(DistancesMatrix& distances, Vertex min = 0, Vertex max = 0) const;

    // calculate the sum of shortest distances between every pair of vertices between min and max, using precalculated distances matrix; max == 0 means max == vertexCount
    Distance calculateSumOfShortestDistances(DistancesMatrix& distances, Vertex min = 0, Vertex max = 0) const;

    // calculate the sum of shortest distances in graph using BFS algorithm
    virtual Distance getSumOfShortestDistancesBFS() const = 0;

    // calculate the sum of shortest distances in graph using Floyd-Warshall algorithm
    Distance getSumOfShortestDistancesFW() const;

    // prints out the graph to stdout, as adjacency lists
    virtual const void printAdjacencyList() const = 0;

    // print graph as a list of vertices and a list of edges, for use with https://csacademy.com/app/graph_editor/
    virtual const void printGraph() const = 0;
};
