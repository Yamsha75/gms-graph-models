#include <algorithm>
#include <cstdio>
#include <queue>

#include "mtx_graph.hpp"


MatrixGraph::MatrixGraph(size_t size) : size(size) {
    edges = new bool* [size];

    for (size_t i = 0; i < size; i++)
        edges[i] = new bool[size](); // init false
}

MatrixGraph::MatrixGraph(const MatrixGraph& other) : MatrixGraph(other.vertexCount) {
    vertexCount = other.vertexCount;

    for (size_t a = 0; a < vertexCount; a++)
        std::copy(other.edges[a], other.edges[a] + vertexCount, edges[a]);
}

MatrixGraph::~MatrixGraph() {
    for (size_t i = 0; i < size; i++)
        delete[] edges[i];

    delete[] edges;
}

size_t MatrixGraph::addVertex(const std::vector<size_t>& neighbours) {
    size_t v = vertexCount++;

    for (size_t const& n : neighbours) {
        edges[v][n] = true;
        edges[n][v] = true;
    }

    return v;
}

void MatrixGraph::addEdge(size_t a, size_t b) {
    edges[a][b] = true;
    edges[b][a] = true;
}

bool MatrixGraph::areVerticesNeighbours(size_t a, size_t b) const {
    return edges[a][b];
}

size_t MatrixGraph::merge(const MatrixGraph& other) {
    size_t offset = vertexCount;

    vertexCount += other.vertexCount;

    for (size_t a = 0; a < other.vertexCount; a++)
        for (size_t b = a + 1; b < other.vertexCount; b++)
            if (other.edges[a][b]) {
                edges[a + offset][b + offset] = true;
                edges[b + offset][a + offset] = true;
            }

    return offset;
}

unsigned int MatrixGraph::calculateBFS() const {
    // iinit
    unsigned int sum = 0;

    std::queue<size_t> open = {};
    bool* visited = new bool[vertexCount];
    unsigned int* distances = new unsigned int[vertexCount];

    // calculate shortest distances for every pair of vertices
    for (size_t s = 0; s < vertexCount; s++) {
        open.push(s);
        std::fill_n(visited, vertexCount, false);
        visited[s] = true;
        distances[s] = 0;

        while (!open.empty()) {
            size_t a = open.front();
            open.pop();

            unsigned int distance = distances[a] + 1;

            bool* neighbours = edges[a];

            for (size_t b = 0; b < vertexCount; b++)
                if (neighbours[b] and not visited[b]) {
                    open.push(b);
                    visited[b] = true;
                    distances[b] = distance;
                }
        }

        for (size_t v = 0; v < vertexCount; v++)
            sum += distances[v];
    }

    // cleanup
    delete[] visited;
    delete[] distances;

    // return
    return sum / 2;
}

unsigned int MatrixGraph::calculateFW() const {
    // init
    const unsigned int max = std::numeric_limits<unsigned int>::max() / 3;

    unsigned int** distances = new unsigned int* [vertexCount];

    for (size_t a = 0; a < vertexCount; a++) {
        unsigned int* temp = new unsigned int[vertexCount];

        distances[a] = temp;

        // set all distances from <a> to <max> value
        std::fill_n(temp, vertexCount, max);

        // set distance from <a> to <a> to 0
        distances[a][a] = 0;

        // set all distances from <a> to each neighbour to 1
        for (size_t b = 0; b < vertexCount; b++)
            if (edges[a][b])
                distances[a][b] = 1;
    }

    // calculate shortest distances for every pair of vertices
    for (size_t k = 0; k < vertexCount; k++)
        for (size_t i = 0; i < vertexCount; i++)
            if (distances[i][k] != max)
                for (size_t j = i + 1; j < vertexCount; j++)
                    if (distances[i][j] > distances[i][k] + distances[k][j])
                        distances[i][j] = distances[j][i] = distances[i][k] + distances[k][j];

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

void MatrixGraph::printEdges() const {
    for (size_t a = 0; a < vertexCount; a++) {
        bool* neighbours = edges[a];

        for (size_t b = a + 1; b < vertexCount; b++)
            if (neighbours[b])
                printf("%zu %zu\n", a, b);
    }
}