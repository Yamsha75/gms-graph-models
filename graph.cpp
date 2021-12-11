#include "graph.hpp"

#include <stdio.h>
#include <queue>

Graph::Graph(size_t vertexCount) : edges(vertexCount), vertexCount(vertexCount) {
    for (size_t i = 0; i < vertexCount; i++)
        edges[i] = {};
}

Graph::Graph(const Edges& e) : Graph(e.size()) {
    for (size_t i = 0; i < vertexCount; i++)
        edges[i] = Neighbours(e[i]);
}

Graph::~Graph() {}

Vertex Graph::addVertex() {
    Vertex v = vertexCount++;

    if (v < edges.size())
        edges[v] = {};
    else
        edges.push_back({});

    return v;
}

Vertex Graph::addVertex(const Neighbours& neighbours) {
    Vertex v = vertexCount++;

    edges.push_back(Neighbours(neighbours));

    for (auto const& n : neighbours)
        edges[n].insert(v);

    return v;
}

void Graph::addEdge(Vertex a, Vertex b) {
    edges[a].insert(b);
    edges[b].insert(a);
}

Vertex Graph::mergeGraph(const Graph& o) {
    Vertex offset = vertexCount;

    // add all vertices and edges from other graph, creating two disconnected graphs
    if (edges.size() < vertexCount + o.vertexCount)
        edges.resize(vertexCount + o.vertexCount);

    for (size_t i = 0; i < o.vertexCount; i++) {
        Vertex v = offset + i;

        edges[v] = {};

        for (auto const& n : o.edges[i])
            edges[v].insert(n + offset);
    }

    vertexCount += o.vertexCount;

    return offset;
}

bool Graph::areVerticesNeighbours(Vertex a, Vertex b) {
    auto neighbours = edges[a];

    return neighbours.find(b) != neighbours.end();
}

void Graph::calculateDistancesToRootBFS(DistancesMatrix& distances, Vertex limit) {
    // limit == 0 means no limit (all vertices)
    if (limit == 0)
        limit = vertexCount;

    // calculate shortest distances from root to each vertex smaller than limit using simple BFS
    std::set<Vertex> visited = {};
    std::queue<Vertex> toVisit = {};

    toVisit.push(0);
    toVisit.push(limit);

    Distance d = 0;

    while (!toVisit.empty()) {
        Vertex v = toVisit.front();
        toVisit.pop();

        if (v == limit) {
            d++;
            if (!toVisit.empty())
                toVisit.push(limit);
        }
        else {
            if (visited.find(v) != visited.end())
                continue;

            visited.insert(v);
            distances[0][v] = d;

            for (auto const& n : edges[v])
                if (n < limit and visited.find(n) == visited.end())
                    toVisit.push(n);
        }
    }
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

Distance Graph::calculateSumOfDistances(DistancesMatrix& distances) {
    Distance sum = 0;

    for (Vertex a = 0; a < vertexCount; a++)
        for (Vertex b = a + 1; b < vertexCount; b++)
            sum += distances[a][b];

    return sum;
}

const void Graph::print() {
    printf("Graph:\n");

    for (size_t i = 0; i < vertexCount; i++) {
        printf("%lu:", i);

        for (auto const& vertex : edges[i])
            printf(" %lu", vertex);

        printf("\n");
    }
}