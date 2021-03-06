#include <algorithm>
#include <cstdio>
#include <queue>

#include "lst_graph.hpp"


ListGraph::ListGraph(const ListGraph& other) : ListGraph(other.vertexCount) {
    vertexCount = other.vertexCount;

    for (size_t a = 0; a < vertexCount; a++)
        edges[a] = Neighbours(other.edges[a]);
}

size_t ListGraph::addVertex(const std::vector<size_t>& neighbours) {
    size_t v = vertexCount++;

    for (size_t const& n : neighbours) {
        edges[v].push_back(n);
        edges[n].push_back(v);
    }

    return v;
}

void ListGraph::addEdge(size_t a, size_t b) {
    edges[a].push_back(b);
    edges[b].push_back(a);
}

bool ListGraph::areVerticesNeighbours(size_t a, size_t b) const {
    for (size_t const& n : edges[a])
        if (n == b)
            return true;

    return false;
}

const ListGraph::Neighbours ListGraph::getVertexNeighbours(size_t v) const {
    return edges[v];
}

size_t ListGraph::merge(const ListGraph& other) {
    size_t offset = vertexCount;

    vertexCount += other.vertexCount;

    for (size_t v = 0; v < other.vertexCount; v++)
        for (size_t const& n : other.edges[v])
            if (n > v) // skip doubling edges
                addEdge(v + offset, n + offset);

    return offset;
}

unsigned int ListGraph::calculateBFS() const {
    // init
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
            size_t v = open.front();
            open.pop();

            unsigned int distance = distances[v] + 1;

            for (size_t const& n : edges[v])
                if (!visited[n]) {
                    open.push(n);
                    visited[n] = true;
                    distances[n] = distance;
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

unsigned int ListGraph::calculateFW() const {
    // init
    const unsigned int max = std::numeric_limits<unsigned int>::max() / 3;

    unsigned int** distances = new unsigned int* [vertexCount];

    for (size_t v = 0; v < vertexCount; v++) {
        unsigned int* temp = new unsigned int[vertexCount];

        distances[v] = temp;

        std::fill_n(temp, vertexCount, max); // init all to <max> value
    }

    for (size_t a = 0; a < vertexCount; a++) {
        distances[a][a] = 0;

        for (size_t const& b : edges[a])
            distances[a][b] = 1;
    }

    // calculate shortest distances for every pair of vertices
    for (size_t k = 0; k < vertexCount; k++)
        for (size_t i = 0; i < vertexCount; i++)
            if (distances[i][k] != max)
                for (size_t j = i + 1; j < vertexCount; j++)
                    if (distances[i][j] > distances[i][k] + distances[k][j]) {
                        distances[i][j] = distances[i][k] + distances[k][j];
                        distances[j][i] = distances[i][k] + distances[k][j];
                    }

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

void ListGraph::printEdges() const {
    for (size_t v = 0; v < vertexCount; v++)
        for (size_t const& n : edges[v])
            if (n > v) // skip double printing edges
                printf("%zu %zu\n", v, n);
}
