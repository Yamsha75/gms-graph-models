#include "graph.hpp"

#include <stdio.h>
#include <queue>

Graph::Graph() {
#ifdef DEBUG
    printf("init graph %p\n", (void*)this);
#endif

    edges = {};
};

Graph::Graph(const Graph& other) {
    Graph();

    vertexCount = other.vertexCount;

    for (auto const& pair : other.edges)
        edges[pair.first] = pair.second;
}

Graph::Graph(const std::list<std::set<Vertex>> vertices) {
    Graph();

    Vertex vertex = 0;
    for (auto const& vertexEdges : vertices)
        edges[vertex++] = vertexEdges;

    vertexCount = vertex;
}

Graph::~Graph(void) {
#ifdef DEBUG
    printf("de-init graph %p\n", (void*)this);
#endif
}

unsigned long Graph::getVertexCount() {
    return vertexCount;
}

const std::map<Vertex, std::set<Vertex>> Graph::getEdges() {
    return edges;
}

Vertex Graph::addVertex() {
    Vertex vertex = vertexCount++;

    edges[vertex] = {};

    return vertex;
}

Vertex Graph::addVertex(const std::set<Vertex> vertexEdges) {
    Vertex vertex = vertexCount++;

    edges[vertex] = vertexEdges;

    for (auto const& neighbour : vertexEdges)
        edges[neighbour].insert(vertex);

    return vertex;
}

bool Graph::addEdge(Vertex a, Vertex b) {
#ifdef DEBUG
    printf("new edge: %lu - %lu\n", a, b);
#endif

    edges[a].insert(b);
    edges[b].insert(a);

    return true;
}

Vertex Graph::mergeGraph(const Graph other) {
    Vertex offset = getVertexCount();

    // add all vertices and edges from other graph, creating two disconnected graphs
    for (auto const& pair : other.edges) {
        Vertex vertex = pair.first + offset;

        edges[vertex] = {};

        for (auto const& neighbour : pair.second)
            edges[vertex].insert(neighbour + offset);
    }

    // update vertexCount
    vertexCount += other.vertexCount;

    return offset;
}

bool Graph::areVerticesNeighbours(Vertex a, Vertex b) {
    auto neighbours = edges[a];

    return neighbours.find(b) != neighbours.end();
}

unsigned long Graph::calculateSumOfDistances() {
    std::map<Vertex, std::map<Vertex, long>> dist = {};

    // initialize distances for each pair
    for (Vertex i = 0; i < vertexCount; i++) {
        dist[i] = {};
        dist[i][i] = 0;
    }

    for (Vertex i = 0; i < vertexCount; i++) {
        for (Vertex j = i + 1; j < vertexCount; j++) {
            if (areVerticesNeighbours(i, j)) {
                // direct neighbours
                dist[i][j] = 1;
                dist[j][i] = 1;
            }
            else {
                dist[i][j] = inf;
                dist[j][i] = inf;
            }
        }
    }

    // calculate shortest distance between each pair
    for (Vertex k = 0; k < vertexCount; k++) {
        for (Vertex i = 0; i < vertexCount; i++) {
            for (Vertex j = 0; j < vertexCount; j++) {
                if (dist[i][j] > dist[i][k] + dist[k][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }

    // sum all distances
    unsigned long sum = 0;

    for (Vertex i = 0; i < vertexCount; i++) {
        for (Vertex j = i + 1; j < vertexCount; j++)
            sum += dist[i][j];
    }

    return sum;
}

void Graph::print() {
    // printf("Graph %p:\n", (void*)this);
    printf("Graph:\n");

    std::set<Vertex> visitedVertices = {};

    for (auto const& pair : edges) {
        if (visitedVertices.find(pair.first) == visitedVertices.end()) {
            printf("  %lu:", pair.first);

            for (auto const& vertex : pair.second) {
                printf(" %lu,", vertex);
            }

            printf("\n");
        }
    }
}