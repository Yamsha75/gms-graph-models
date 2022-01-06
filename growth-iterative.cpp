#include <algorithm>
#include <cmath>
#include <list>

#include "growth-iterative.hpp"
#include "lst_graph.hpp"


GrowthIterative::GrowthIterative(size_t iterations, size_t newVertices, size_t vertexCount) {
    graph = new ListGraph(vertexCount);

    // step 0
    graph->addVertex();
    graph->addVertex({ 0 });

    // edges created in k-1
    std::list<Edge> edges = { Edge(0, 1) };

    // shortest distances between each pair of vertices
    std::vector<std::vector<unsigned int>> distances(vertexCount, std::vector<unsigned int>(vertexCount));
    distances[0][1] = 1;
    distances[1][0] = 1;

    // further steps
    for (unsigned short int step = 1; step <= iterations; step++) {
        std::list<Edge> newEdges = {};

        for (auto const& edge : edges) {
            std::vector<size_t> neighbours = { edge.first, edge.second };

            for (size_t i = 0; i < newVertices; i++) {
                size_t v = graph->addVertex(neighbours);

                distances[edge.first][v] = 1;
                distances[edge.second][v] = 1;

                distances[v][edge.first] = 1;
                distances[v][edge.second] = 1;

                for (size_t j = 0; j < v; j++) {
                    if (j == edge.first or j == edge.second)
                        continue;

                    unsigned int distance = std::min(distances[j][edge.first], distances[j][edge.second]) + 1;
                    distances[j][v] = distance;
                    distances[v][j] = distance;
                }

                newEdges.push_back(Edge(edge.first, v));
                newEdges.push_back(Edge(edge.second, v));
            }
        }

        edges = newEdges;
    }

    // precalculate sum of shortest distances
    for (size_t a = 0; a < vertexCount; a++)
        for (size_t b = a + 1; b < vertexCount; b++)
            sumOfShortestDistances += distances[a][b];
}

GrowthIterative::GrowthIterative(size_t iterations, size_t newVertices)
    : GrowthIterative(iterations, newVertices, calculateNumberOfVertices(iterations, newVertices)) {};

size_t GrowthIterative::calculateNumberOfVertices(size_t k, size_t r) {
    return (r * (3 + (size_t)pow(2.0 * (double)r, (double)k)) - 2) / (2 * r - 1);
}

unsigned int GrowthIterative::calculate() const {
    return sumOfShortestDistances;
}
