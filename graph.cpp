#include "graph.hpp"

#include <stdio.h>
#include <queue>

Graph::Graph() {
    edges = {};
};

Graph::Graph(const Graph& o) : Graph() {
    vertexCount = o.vertexCount;

    for (auto const& pair : o.edges)
        edges[pair.first] = pair.second;
}

Graph::Graph(const std::list<std::set<vertex>>& listOfVertexEdges) : Graph() {
    for (auto const& vertexEdges : listOfVertexEdges)
        edges[vertexCount++] = std::set<vertex>(vertexEdges);
}

Graph::~Graph(void) {}

vertex Graph::addVertex() {
    vertex v = vertexCount++;

    edges[v] = {};

    return v;
}

vertex Graph::addVertex(const std::set<vertex>& vertexEdges) {
    vertex v = vertexCount++;

    edges[v] = std::set<vertex>(vertexEdges);

    for (auto const& n : vertexEdges)
        edges[n].insert(v);

    return v;
}

void Graph::addEdge(vertex a, vertex b) {
    edges[a].insert(b);
    edges[b].insert(a);
}

vertex Graph::mergeGraph(const Graph& o) {
    vertex offset = vertexCount;

    // add all vertices and edges from other graph, creating two disconnected graphs
    for (auto const& pair : o.edges) {
        vertex v = pair.first + offset;

        edges[v] = {};

        for (auto const& neighbour : pair.second)
            edges[v].insert(neighbour + offset);
    }

    // update vertexCount
    vertexCount += o.vertexCount;

    return offset;
}

bool Graph::areVerticesNeighbours(vertex a, vertex b) {
    auto neighbours = edges[a];

    return neighbours.find(b) != neighbours.end();
}

void Graph::calculateDistancesToRootBFS(std::map<vertex, std::map<vertex, distance>>& distances, vertex limit) {
    // limit == 0 means no limit (all vertices)
    if (limit == 0)
        limit = vertexCount;

    // calculate shortest distances from root to each vertex smaller than limit using simple BFS
    std::set<vertex> visited = {};
    std::queue<vertex> toVisit = {};

    toVisit.push(0);
    toVisit.push(limit);

    distance d = 0;

    while (!toVisit.empty()) {
        vertex v = toVisit.front();
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

void Graph::calculateAllDistancesFW(std::map<vertex, std::map<vertex, distance>>& distances, bool skipInit) {
    if (!skipInit)
        // initial distances for self and neighbours for each vertex
        for (vertex a = 0; a < vertexCount; a++) {
            distances[a][a] = 0;

            for (vertex b = a + 1; b < vertexCount; b++) {
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
    for (vertex k = 0; k < vertexCount; k++)
        for (vertex i = 0; i < vertexCount; i++)
            for (vertex j = 0; j < vertexCount; j++)
                if (distances[i][j] > distances[i][k] + distances[k][j])
                    distances[i][j] = distances[i][k] + distances[k][j];

    // done
}

distance Graph::calculateSumOfDistances(std::map<vertex, std::map<vertex, distance>>& distances) {
    distance sum = 0;

    for (vertex a = 0; a < vertexCount; a++)
        for (vertex b = a + 1; b < vertexCount; b++) {
            distance d = distances[a][b];
            if (d == inf)
                printf("distance between %lu and %lu couldn't be calculated!\n", a, b);
            sum += d;
        }

    return sum;
}

const void Graph::print() {
    printf("Graph:\n");

    std::set<vertex> visited = {};

    for (auto const& pair : edges)
        if (visited.find(pair.first) == visited.end()) {
            printf("  %lu:", pair.first);

            for (auto const& vertex : pair.second)
                printf(" %lu", vertex);

            printf("\n");
        }
}