#pragma once

#include <vector>

#include "graph.hpp"
#include "al_graph.hpp"


#define BARABASI 0

// vector of ternary (base3) digits
typedef std::vector<unsigned short int> Ternary;


class Barabasi {
private:
    const size_t iterations;
    std::vector<Ternary> ternaryVertices;

    // return vertex ID in ternary
    Ternary toTernary(Vertex v) const;

    // calculate distance from vertex v to root, using ternary representation
    Distance calculateDistanceToRoot(Vertex v) const;

    // calculate distance between vertices a and b, using ternary representation
    Distance calculateDistanceBetweenVertices(Vertex a, Vertex b) const;

    // calculate distances between root and vertices with min <= ID < max, using ternary
    // representation, and save to distances matrix
    void calculateDistancesFromRoot(DistancesMatrix& distances, Vertex min, Vertex max) const;

public:
    ALGraph graph;

    Barabasi(size_t iterations);

    // calculate sum of shortest paths between every pair of vertices
    Distance calculateSumOfShortestDistances() const;

    // print graph as a list of vertices and a list of edges, using ternary representation, for use with https://csacademy.com/app/graph_editor/
    const void printGraph() const;
};
