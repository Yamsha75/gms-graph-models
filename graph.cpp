#include <algorithm>
#include <queue>
#include <stdio.h>

#include "graph.hpp"


Graph::Graph(size_t size) {
    this->size = size;

    edges = new bool* [size];

    for (size_t i = 0; i < size; i++)
        edges[i] = new bool[size]();
}

Graph::Graph(const Graph& other) : Graph(other.vertexCount) {
    vertexCount = other.vertexCount;

    for (size_t a = 0; a < vertexCount; a++)
        for (size_t b = a + 1; b < vertexCount; b++)
            if (other.edges[a][b]) {
                edges[a][b] = true;
                edges[b][a] = true;
            }
}

Graph::~Graph() {
    for (size_t i = 0; i < size; i++)
        delete[] edges[i];

    delete[] edges;
}

size_t Graph::addVertex() {
    return vertexCount++;
}

size_t Graph::addVertex(const std::vector<size_t>& neighbours) {
    size_t v = vertexCount++;

    for (size_t const& n : neighbours) {
        edges[v][n] = true;
        edges[n][v] = true;
    }

    return v;
}

void Graph::addEdge(size_t a, size_t b) {
    edges[a][b] = true;
    edges[b][a] = true;
}

bool Graph::areVerticesNeighbours(size_t a, size_t b) const {
    return edges[a][b];
}

size_t Graph::merge(const Graph& other) {
    size_t offset = vertexCount;

    for (size_t a = 0; a < other.vertexCount; a++)
        for (size_t b = a + 1; b < other.vertexCount; b++)
            if (other.edges[a][b]) {
                edges[a + offset][b + offset] = true;
                edges[b + offset][a + offset] = true;
            }

    vertexCount += other.vertexCount;

    return offset;
}

unsigned int Graph::calculateBFS() const {
    // init
    unsigned int sum = 0;

    std::queue<size_t> open = {};
    bool* visited = new bool[vertexCount];

    unsigned int d;
    size_t a;

    // calculate shortest distances for every pair of vertices
    for (size_t s = 0; s < vertexCount; s++) {
        std::fill(visited, visited + vertexCount, false);

        open.push(s);
        open.push(vertexCount);

        d = 0;

        while (!open.empty()) {
            a = open.front();
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

                for (size_t b = 0; b < vertexCount; b++)
                    if (!visited[b] and edges[a][b])
                        open.push(b);
            }
        }
    }

    // cleanup
    delete[] visited;

    // return
    return sum / 2;
}

unsigned int Graph::calculateFW() const {
    // init
    const unsigned int max = std::numeric_limits<unsigned int>::max() / 3;

    unsigned int** distances = new unsigned int* [vertexCount];

    for (size_t v = 0; v < vertexCount; v++)
        distances[v] = new unsigned int[vertexCount]();

    for (size_t a = 0; a < vertexCount; a++) {
        for (size_t b = a + 1; b < vertexCount; b++) {
            if (areVerticesNeighbours(a, b)) {
                distances[a][b] = 1;
                distances[b][a] = 1;
            }
            else {
                distances[a][b] = max;
                distances[b][a] = max;
            }
        }
    }

    // calculate shortest distances for every pair of vertices
    for (size_t k = 0; k < vertexCount; k++)
        for (size_t i = 0; i < vertexCount; i++)
            for (size_t j = 0; j < vertexCount; j++)
                if (distances[i][j] > distances[i][k] + distances[k][j])
                    distances[i][j] = distances[i][k] + distances[k][j];

    // calculate sum of shortest distances between every pair of vertices
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

void Graph::print() const {
    for (size_t v = 0; v < vertexCount; v++)
        printf("%zu\n", v);

    for (size_t a = 0; a < vertexCount; a++)
        for (size_t b = a + 1; b < vertexCount; b++)
            if (edges[a][b])
                printf("%zu %zu\n", a, b);
}
