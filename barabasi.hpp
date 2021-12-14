#pragma once

#include <vector>

#include "graph.hpp"


#define BARABASI 0

// vector of ternary (base3) digits
typedef std::vector<unsigned short int> Ternary;


class Barabasi {
private:
    Graph graph;
    size_t iterations;
    std::vector<Ternary> ternaryVertices;

public:
    // return vertex ID in ternary
    Ternary toTernary(Vertex v);

    // return vertex ID from ternary
    // const Vertex toDecimal(Ternary t, size_t skipDigits = 0);

    Barabasi(size_t iterations);

    ~Barabasi();

    // calculate distance from vertex v to root, using ternary representation
    Distance calculateDistanceToRoot(Vertex v);

    // calculate distance between vertices a and b, using ternary representation
    Distance calculateDistanceBetweenVertices(Vertex a, Vertex b);

    // calculate distances between root and vertices with min <= ID < max, using ternary
    // representation, and save to distances matrix
    void calculateDistancesFromRoot(DistancesMatrix& distances, Vertex min, Vertex max);

    // calculate sum of shortest paths between every pair of vertices
    Distance calculateSumOfDistances();

    // print graph, using ternary representation, as a list of vertices and a list of edges
    // for use with https://csacademy.com/app/graph_editor/
    void print(); 
};
