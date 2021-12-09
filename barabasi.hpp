#include "graph.hpp"

#include <vector>
#include <map>

#define BARABASI 0

typedef std::vector<unsigned short int> ternary; // vector of ternary (base3) digits

class Barabasi {
private:
    Graph graph;
    unsigned short int iterations;
    std::map<vertex, ternary> ternaryVertices;
public:
    ternary toTernary(vertex v); // return vertex ID in base3
    const vertex toDecimal(ternary t, unsigned short int skipDigits = 0); // return vertex ID from ternary

    Barabasi(unsigned short int iterations);

    ~Barabasi();

    distance calculateShortestPathFromRoot(vertex f);
    distance calculateSumOfDistances();

    void printGraph() {graph.print();};
    void print(); // for use with https://csacademy.com/app/graph_editor/
};
