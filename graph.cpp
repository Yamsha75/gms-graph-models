#include "graph.hpp"


Vertex Graph::addVertex() {
    return vertexCount++;
}

Distance Graph::calculateSumOfShortestDistances(DistancesMatrix& distances, Vertex min, Vertex max) const {
    if (max == 0)
        max = vertexCount;

    Distance sum = 0;

    for (Vertex a = min; a < max; a++)
        for (Vertex b = a + 1; b < max; b++)
            sum += distances[a][b];

    return sum;
}

void Graph::calculateShortestDistancesBetweenVerticesBFS(DistancesMatrix& distances, Vertex min, Vertex max) const {
    if (max == 0)
        max = vertexCount;

    for (Vertex s = min; s < max; s++)
        calculateShortestDistancesFromVertexBFS(distances, s, min, max);
}

Distance Graph::getSumOfShortestDistancesFW() const {
    // init distances matrix
    DistancesMatrix distances = DistancesMatrix(vertexCount);

    for (Vertex v = 0; v < vertexCount; v++)
        distances[v] = std::vector<Distance>(vertexCount);

    // load initial distances, based on edges
    for (Vertex a = 0; a < vertexCount; a++) {
        distances[a][a] = 0;

        for (Vertex b = a + 1; b < vertexCount; b++) {
            if (areVerticesNeighbours(a, b)) {
                distances[a][b] = 1;
                distances[b][a] = 1;
            }
            else {
                distances[a][b] = inf;
                distances[b][a] = inf;
            }
        }
    }

    // calculate shortest distances for every pair of vertices
    for (Vertex k = 0; k < vertexCount; k++)
        for (Vertex i = 0; i < vertexCount; i++)
            for (Vertex j = 0; j < vertexCount; j++)
                if (distances[i][j] > distances[i][k] + distances[k][j])
                    distances[i][j] = distances[i][k] + distances[k][j];

    // calculate the sum of shortest distances and return it
    return calculateSumOfShortestDistances(distances, 0, vertexCount);
}