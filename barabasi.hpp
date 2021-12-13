#pragma once

#include <vector>

#include "graph.hpp"


#define BARABASI 0


typedef std::vector<unsigned short int> Ternary; // vector of ternary (base3) digits


class Barabasi {
private:
    Graph graph;
    unsigned short int iterations;
    std::vector<Ternary> ternaryVertices;

public:
    Ternary toTernary(Vertex v); // return vertex ID in ternary
    const Vertex toDecimal(Ternary t, size_t skipDigits = 0); // return vertex ID from ternary

    Barabasi(unsigned short int iterations);

    ~Barabasi();

    Distance calculateShortestPathFromRoot(Vertex f);
    Distance calculateSumOfDistances();

    void printGraph() { graph.print(); };
    void print(); // for use with https://csacademy.com/app/graph_editor/
};
