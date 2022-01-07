#include <algorithm>
#include <cmath>
#include <cstring>

#include "kronecker.hpp"
#include "mtx_graph.hpp"


Kronecker::Kronecker(size_t iterations, const char* adjacencyMatrix, size_t baseVertexCount)
    : baseVertexCount(baseVertexCount)
{
    size_t vertexCount = (size_t)pow((double)baseVertexCount, (double)iterations);

    graph = new MatrixGraph(vertexCount);

    MatrixGraph* g = dynamic_cast<MatrixGraph*>(graph);

    // recreate base graph from given adjacency matrix
    for (size_t v = 0; v < baseVertexCount; v++)
        g->addVertex();

    for (size_t a = 0; a < baseVertexCount; a++)
        for (size_t b = a + 1; b < baseVertexCount; b++)
            if (adjacencyMatrix[a * baseVertexCount + b] == '1')
                g->addEdge(a, b);

    // apply strong product <iterations - 1> times
    MatrixGraph H = MatrixGraph(*g);
    size_t hCount = H.len();

    for (size_t k = 2; k <= iterations; k++) {
        MatrixGraph G = MatrixGraph(*g);

        size_t gCount = G.len();

        // for each vertex x, y:
        // - x1, y1 are vertex IDs in G
        // - x2, y2 are vertex IDs in H

        // "vertical" edges
        for (size_t n = 1; n < hCount; n++)
            g->merge(G);

        for (size_t x2 = 0; x2 < hCount; x2++)
            for (size_t y2 = x2 + 1; y2 < hCount; y2++)
                if (H.areVerticesNeighbours(x2, y2)) {
                    // "horizontal" edges
                    for (size_t v = 0; v < gCount; v++)
                        g->addEdge(v + x2 * gCount, v + y2 * gCount);

                    // "diagonal" edges
                    for (size_t x1 = 0; x1 < gCount; x1++) {
                        size_t x = x1 + x2 * gCount;

                        for (size_t y1 = 0; y1 < gCount; y1++)
                            if (x1 != y1 and G.areVerticesNeighbours(x1, y1)) {
                                size_t y = y1 + y2 * gCount;

                                if (not graph->areVerticesNeighbours(y, x))
                                    g->addEdge(x, y);
                            }
                    }
                }
    }
}


Kronecker::Kronecker(size_t iterations, const char* adjacencyMatrix)
    : Kronecker(iterations, adjacencyMatrix, (size_t)sqrt((double)strlen(adjacencyMatrix))) {};

unsigned int Kronecker::calculate() const {
    // init
    const unsigned int max = std::numeric_limits<unsigned int>::max() / 3;

    size_t vertexCount = graph->len();

    unsigned int** distances = new unsigned int* [vertexCount];

    for (size_t a = 0; a < vertexCount; a++) {
        unsigned int* temp = new unsigned int[vertexCount];

        distances[a] = temp;

        // set all distances from max value
        std::fill_n(temp, vertexCount, max);
    }

    for (size_t a = 0; a < vertexCount; a++) {
        // set distance to self
        distances[a][a] = 0;

        // set all distances to each neighbour
        for (size_t b = a + 1; b < vertexCount; b++)
            if (graph->areVerticesNeighbours(a, b))
                distances[a][b] = distances[b][a] = 1;
    }

    // simplified Floyd-Warshall algorithm for vertices belonging to the original input graph
    for (size_t k = 0; k < baseVertexCount; k++)
        for (size_t a = 0; a < baseVertexCount; a++)
            if (distances[a][k] != max)
                for (size_t b = a + 1; b < baseVertexCount; b++)
                    if (distances[a][b] > distances[a][k] + distances[k][b])
                        distances[a][b] = distances[b][a] = distances[a][k] + distances[k][b];

    // distances between all other vertex pairs can be simplified to paths in the original input graph
    for (size_t a = 0; a < vertexCount; a++)
        for (size_t b = a + 1; b < vertexCount; b++) {
            size_t a1 = a % baseVertexCount;
            size_t a2 = a / baseVertexCount;

            size_t b1 = b % baseVertexCount;
            size_t b2 = b / baseVertexCount;

            // a1, b1 are vertices in input graph on the left side of strong product operation
            // a2, b2 are vertices in input graph on the right side of strong product operation

            if (a1 == b1)
                // path a-b can be simplified to a2-b2
                distances[a][b] = distances[b][a] = distances[a2][b2];
            else if (a2 == b2)
                // path a-b can be simplified to a1-b1
                distances[a][b] = distances[b][a] = distances[a1][b1];
            else
                // path a-b can be simplified to max(a1-b1, a2-b2)
                distances[a][b] = distances[b][a] = std::max(distances[a1][b1], distances[a2][b2]);
        }

    unsigned int result = 0;

    for (size_t a = 0; a < vertexCount; a++)
        for (size_t b = a + 1; b < vertexCount; b++)
            result += distances[a][b];

    // cleanup
    for (size_t v = 0; v < vertexCount; v++) {
        delete[] distances[v];
    }

    delete[] distances;

    // return
    return result;
}
