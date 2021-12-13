#pragma once

#include <limits>
#include <set>
#include <vector>


typedef size_t Vertex;
typedef std::set<Vertex> Neighbours;
typedef std::vector<Neighbours> Edges;

typedef unsigned long int Distance;
typedef std::vector<std::vector<Distance>> DistancesMatrix;


class Graph {
public:
    static const Distance inf = std::numeric_limits<Distance>::max() / 2;

    Vertex maxVertexCount;
    Vertex vertexCount = 0;
    Edges edges;

    Graph(Vertex maxVertexCount);

    // copy constructor
    Graph(const Graph& o, Vertex maxVertexCount = 0);

    ~Graph();

    // add a new vertex without any edges; returns new vertex ID
    Vertex addVertex();

    // add a new vertex with preexisint edges; returns new vertex ID
    Vertex addVertex(const Neighbours& neighbours);

    // add a new edge between existing edges
    void addEdge(Vertex a, Vertex b);

    // adds other graph to this, with no edges between; effectively two disjointed graphs
    Vertex mergeGraph(const Graph& o);

    bool areVerticesNeighbours(Vertex a, Vertex b);

    // calculate distances between vertex s and every other with ID between min and max;
    // max == 0 means max = vertexCount
    void calculateDistancesFromVertexBFS(DistancesMatrix& distances, Vertex s = 0, Vertex min = 0, Vertex max = 0);

    // calculate distances between every pair of vertices with IDs between min and max;
    // max == 0 means max = vertexCount
    void calculateDistancesBetweenVerticesBFS(DistancesMatrix& distances, Vertex min = 0, Vertex max = 0);

    // calculate the sum of distances between every pair of vertices with IDs between min and max,
    // using precalculated distances; max == 0 means max = vertexCount
    Distance calculateSumOfDistances(DistancesMatrix& distances, Vertex min = 0, Vertex max = 0);

    // calculate the sum of distances between every pair of vertices, using a simple BFS algorithm
    Distance calculateSumOfDistancesBFS();

    // calculate the sum of distances between every pair of vertices, using the Floyd-Warshall algorithm
    Distance calculateSumOfDistancesFW();

    // prints out the graph to stdout, as adjacency lists
    const void print();
};
