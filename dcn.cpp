#include <cmath>

#include "dcn.hpp"


DCN::DCN(size_t vertexCount) : Model(vertexCount) {
    graph.addVertex();

    std::vector<size_t> parents = {};

    for (size_t id = 1; id < vertexCount; id++) {
        if ((id & 1) == 1) {
            parents.clear();

            size_t parent = getVertexParent(id);
            while (parent != 0) {
                parents.push_back(parent);
                parent = getVertexParent(parent);
            }

            parents.push_back(0);
        }

        graph.addVertex(parents);
    }
}

size_t DCN::getVertexParent(size_t v) {
    return (v - 1) / 2;
}

unsigned int DCN::calculate() const {
    unsigned int sum = 0;

    size_t vertexCount = graph.len();

    for (size_t v = 0; v < vertexCount; v++) {
        size_t neighboursCount = graph.getVertexNeighbours(v).size();

        sum += neighboursCount + (vertexCount - neighboursCount - 1) * 2;
    }

    return sum / 2;
}
