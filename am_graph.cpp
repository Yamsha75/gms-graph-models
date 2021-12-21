#include <stdio.h>
#include <queue>

#include "am_graph.hpp"


AMGraph::AMGraph(Vertex maxVertexCount) : Graph(maxVertexCount) {
    edges = AdjacencyMatrix(maxVertexCount);

    for (Vertex v = 0; v < maxVertexCount; v++)
        edges[v] = Neighbours(maxVertexCount);
}

AMGraph::AMGraph(const AMGraph& other, Vertex maxVertexCount) : AMGraph(std::max(other.vertexCount, maxVertexCount)) {
    vertexCount = other.vertexCount;

    for (Vertex v = 0; v < vertexCount; v++)
        edges[v] = Neighbours(other.edges[v]);
}

Vertex AMGraph::addVertex(const Neighbours& neighbours) {
    Vertex v = Graph::addVertex();

    for (const Vertex& n : neighbours) {
        edges[v][n] = true;
        edges[n][v] = true;
    }

    return v;
}

void AMGraph::addEdge(Vertex a, Vertex b) {
    edges[a][b] = true;
    edges[b][a] = true;
}

bool AMGraph::areVerticesNeighbours(Vertex a, Vertex b) const {
    return edges[a][b];
}

Vertex AMGraph::merge(const AMGraph& other) {
    Vertex offset = vertexCount;

    for (Vertex a = 0; a < other.vertexCount; a++)
        for (Vertex b = a + 1; b < other.vertexCount; b++)
            addEdge(a + offset, b + offset);

    vertexCount += other.vertexCount;

    return offset;
}

void AMGraph::calculateShortestDistancesFromVertexBFS(DistancesMatrix& distances, Vertex s, Vertex min, Vertex max) const {
    if (max == 0)
        max = vertexCount;

    Neighbours visited = Neighbours(max);
    std::queue<Vertex> open = {};

    open.push(s);
    open.push(max);

    Distance d = 0;

    while (!open.empty()) {
        Vertex a = open.front();
        open.pop();

        if (a == max)
            if (!open.empty()) {
                d++;
                open.push(max);
            }
            else if (!visited[a]) {
                visited[a] = true;

                if (a > s) // populates only the upper half of the matrix
                    distances[s][a] = d;

                for (Vertex b = min; b < max; b++)
                    if (!visited[b] and edges[a][b])
                        open.push(b);
            }
    }
}

Distance AMGraph::getSumOfShortestDistancesBFS() const {
    Distance sum = 0;

    std::queue<Vertex> open = {};

    for (Vertex s = 0; s < vertexCount; s++) {
        Neighbours visited = Neighbours(vertexCount);

        open.push(s);
        open.push(vertexCount);

        Distance d = 0;

        while (!open.empty()) {
            Vertex a = open.front();
            open.pop();

            if (a == vertexCount) {
                if (!open.empty()) {
                    d++;
                    open.push(vertexCount);
                }
            }
            else if (!visited[a]) {
                visited[a] = true;

                sum += d;

                for (Vertex b = 0; b < vertexCount; b++)
                    if (!visited[b] and edges[a][b])
                        open.push(b);
            }
        }
    }

    return sum / 2;
}

const void AMGraph::printAdjacencyList() const {
    for (Vertex a = 0; a < vertexCount; a++) {
        printf("%u:", a);

        for (Vertex b = 0; b < vertexCount; b++)
            if (areVerticesNeighbours(a, b))
                printf(" %u", b);

        printf("\n");
    }
}

const void AMGraph::printGraph() const {
    for (Vertex v = 0; v < vertexCount; v++)
        printf("%u\n", v);

    for (Vertex a = 0; a < vertexCount; a++)
        for (Vertex b = a + 1; b < vertexCount; b++)
            if (edges[a][b])
                printf("%u %u\n", a, b);
}
