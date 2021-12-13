#include <stdio.h>
#include <queue>

#include "graph.hpp"


Graph::Graph(Vertex maxVertexCount) : maxVertexCount(maxVertexCount) {
    // init adjacency lists
    edges = Edges(maxVertexCount);

    for (Vertex i = 0; i < maxVertexCount; i++)
        edges[i] = {};
}

Graph::Graph(const Graph& o, Vertex maxVertexCount) : Graph(std::max(o.vertexCount, maxVertexCount)) {
    vertexCount = o.vertexCount;

    for (Vertex i = 0; i < vertexCount; i++)
        edges[i] = Neighbours(o.edges[i]);
}

Graph::~Graph() {}

Vertex Graph::addVertex() {
    return vertexCount++;
}

Vertex Graph::addVertex(const Neighbours& neighbours) {
    Vertex v = vertexCount++;

    edges[v] = Neighbours(neighbours);

    for (auto const& n : neighbours)
        edges[n].insert(v);

    return v;
}

void Graph::addEdge(Vertex a, Vertex b) {
    edges[a].insert(b);
    edges[b].insert(a);
}

Vertex Graph::mergeGraph(const Graph& o) {
    // add all vertices and edges from other graph, creating two disconnected graphs
    Vertex offset = vertexCount;

    for (Vertex v = 0; v < o.vertexCount; v++)
        for (auto const& n : o.edges[v])
            edges[v + offset].insert(n + offset);

    vertexCount += o.vertexCount;

    return offset;
}

bool Graph::areVerticesNeighbours(Vertex a, Vertex b) {
    auto neighbours = edges[a];

    return neighbours.find(b) != neighbours.end();
}

void Graph::calculateDistancesFromVertexBFS(Vertex s, DistancesMatrix& distances, Vertex limit) {
    // limit == 0 means no limit (all vertices)
    if (limit == 0)
        limit = vertexCount;

    // calculate shortest distances from v to each vertex within limit using simple BFS
    std::set<Vertex> visited = {};
    std::queue<Vertex> open = {};

    open.push(s);
    open.push(limit);

    Distance d = 0;

    while (!open.empty()) {
        Vertex v = open.front();
        open.pop();

        if (v == limit) {
            if (!open.empty()) {
                d++;
                open.push(limit);
            }
        }
        else if (visited.find(v) == visited.end()) {
            visited.insert(v);
            distances[std::min(s, v)][std::max(s, v)] = d;

            for (auto const& n : edges[v])
                if (n < limit and visited.find(n) == visited.end())
                    open.push(n);
        }
    }
}

void Graph::calculateDistancesFromRootBFS(DistancesMatrix& distances, Vertex limit) {
    return calculateDistancesFromVertexBFS(0, distances, limit);
}

Distance Graph::calculateSumOfDistances(DistancesMatrix& distances) {
    Distance sum = 0;

    for (Vertex a = 0; a < vertexCount; a++)
        for (Vertex b = a + 1; b < vertexCount; b++)
            sum += distances[a][b];

    return sum;
}

Distance Graph::calculateSumOfDistancesBFS() {
    Distance sum = 0;

    std::queue<Vertex> open = {};
    std::set<Vertex> visited = {};

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

                for (auto const& n : edges[v])
                    if (visited.find(n) == visited.end())
                        open.push(n);
            }
        }

        visited.clear();
    }

    return sum / 2;
}

Distance Graph::calculateSumOfDistancesFW() {
    // init vectors
    DistancesMatrix distances = std::vector<std::vector<Distance>>(vertexCount);

    for (Vertex v = 0; v < vertexCount; v++) {
        distances[v] = std::vector<Distance>(vertexCount);
    }

    // initial distances, based on edges
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

    // calculate shortest distance between each pair of vertices, using Floyd–Warshall algorithm
    for (Vertex k = 0; k < vertexCount; k++)
        for (Vertex i = 0; i < vertexCount; i++)
            for (Vertex j = 0; j < vertexCount; j++)
                if (distances[i][j] > distances[i][k] + distances[k][j])
                    distances[i][j] = distances[i][k] + distances[k][j];

    return calculateSumOfDistances(distances);
}

const void Graph::print() {
    printf("Graph:\n");

    for (Vertex v = 0; v < vertexCount; v++) {
        printf("%lu:", v);

        for (auto const& n : edges[v])
            printf(" %lu", n);

        printf("\n");
    }
}