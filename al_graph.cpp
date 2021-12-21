#include <stdio.h>
#include <vector>
#include <queue>

#include "al_graph.hpp"

ALGraph::ALGraph(Vertex maxVertexCount) : Graph(maxVertexCount) {
    edges = AdjacencyLists(maxVertexCount);

    for (Vertex v = 0; v < maxVertexCount; v++)
        edges[v] = {};
}

ALGraph::ALGraph(const ALGraph& other, Vertex maxVertexCount) : ALGraph(std::max(other.vertexCount, maxVertexCount)) {
    vertexCount = other.vertexCount;

    for (Vertex v = 0; v < vertexCount; v++)
        edges[v] = Neighbours(other.edges[v]);
}

Vertex ALGraph::addVertex(const Neighbours& neighbours) {
    Vertex v = vertexCount++;

    edges[v] = Neighbours(neighbours);

    for (const Vertex& n : neighbours)
        edges[n].insert(v);

    return v;
}

void ALGraph::addEdge(Vertex a, Vertex b) {
    edges[a].insert(b);
    edges[b].insert(a);
}

bool ALGraph::areVerticesNeighbours(Vertex a, Vertex b) const {
    auto neighbours = edges[a];

    return neighbours.find(b) != neighbours.end();
}

Vertex ALGraph::merge(const ALGraph& other) {
    Vertex offset = vertexCount;

    for (Vertex v = 0; v < other.vertexCount; v++)
        for (const Vertex& n : other.edges[v])
            edges[v + offset].insert(n + offset);

    vertexCount += other.vertexCount;

    return offset;
}

void ALGraph::calculateShortestDistancesFromVertexBFS(DistancesMatrix& distances, Vertex s, Vertex min, Vertex max) const {
    if (max == 0)
        max = vertexCount;

    std::set<Vertex> visited = {};
    std::queue<Vertex> open = {};

    open.push(s);
    open.push(max);

    Distance d = 0;

    while (!open.empty()) {
        Vertex v = open.front();
        open.pop();

        if (v == max) {
            if (!open.empty()) {
                d++;
                open.push(max);
            }
        }
        else if (visited.find(v) == visited.end()) {
            visited.insert(v);

            if (v > s) // populates only the upper half of the matrix
                distances[s][v] = d;

            for (const Vertex& n : edges[v])
                if (n >= min and n < max and visited.find(n) == visited.end())
                    open.push(n);
        }
    }
}

Distance ALGraph::getSumOfShortestDistancesBFS() const {
    Distance sum = 0;

    Neighbours visited = {};
    std::queue<Vertex> open = {};

    for (Vertex s = 0; s < vertexCount; s++) {
        open.push(s);
        open.push(vertexCount);

        Distance d = 0;

        while (!open.empty()) {
            Vertex v = open.front();
            open.pop();

            if (v == vertexCount) {
                if (!open.empty()) {
                    d++;
                    open.push(vertexCount);
                }
            }
            else if (visited.find(v) == visited.end()) {
                visited.insert(v);

                sum += d;

                for (const Vertex& n : edges[v])
                    if (visited.find(n) == visited.end())
                        open.push(n);
            }
        }

        visited.clear();
    }

    return sum / 2;
}

const void ALGraph::printAdjacencyList() const {
    for (Vertex v = 0; v < vertexCount; v++) {
        printf("%u:", v);

        for (const Vertex& n : edges[v])
            printf(" %u", n);

        printf("\n");
    }
}

const void ALGraph::printGraph() const {
    for (Vertex v = 0; v < vertexCount; v++)
        printf("%u\n", v);

    for (Vertex v = 0; v < vertexCount; v++)
        for (const Vertex& n : edges[v])
            if (n > v)
                printf("%u %u\n", v, n);
}
