#include <string.h>
#include <math.h>

#include "kronecker.hpp"


Kronecker::Kronecker(size_t iterations, const char* adjacencyMatrix, size_t baseVertexCount) : Model((size_t)pow((double)baseVertexCount, (double)iterations)), baseVertexCount(baseVertexCount) {
    // recreate base graph from given adjacency matrix
    for (size_t v = 0; v < baseVertexCount; v++)
        graph.addVertex();

    for (size_t a = 0; a < baseVertexCount; a++)
        for (size_t b = a + 1; b < baseVertexCount; b++) {
            if (adjacencyMatrix[a * baseVertexCount + b] == '1')
                graph.addEdge(a, b);
        }

    // apply strong product <iterations - 1> times
    Graph H = Graph(graph);

    for (size_t k = 2; k <= iterations; k++) {
        Graph G = Graph(graph);

        size_t gCount = G.len();

        for (size_t n = 1; n < H.len(); n++)
            graph.merge(G);

        // for each vertex x or y:
        // - x1 and y1 are vertex IDs in G
        // - x2 and y2 are vertex IDs in H
        for (size_t x = 0; x < graph.len(); x++)
            for (size_t y = x + 1; y < graph.len(); y++) {
                if (graph.areVerticesNeighbours(x, y))
                    // vertices u and v are already connected
                    continue;

                size_t x1 = x % gCount;
                size_t y1 = y % gCount;

                size_t x2 = x / gCount;
                size_t y2 = y / gCount;

                if ((x1 == y1 and H.areVerticesNeighbours(x2, y2)) or
                    (x2 == y2 and G.areVerticesNeighbours(x1, y1)) or
                    (G.areVerticesNeighbours(x1, y1) and H.areVerticesNeighbours(x2, y2)))
                {
                    graph.addEdge(x, y);
                }
            }
    }
}


Kronecker::Kronecker(size_t iterations, const char* adjacencyMatrix) : Kronecker(iterations, adjacencyMatrix, (size_t)sqrt((double)strlen(adjacencyMatrix))) {};

unsigned int Kronecker::calculate() const {
    return graph.calculateBFS();
}