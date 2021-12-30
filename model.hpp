#pragma once

#include "graph.hpp"


class Model {
protected:
    Graph graph;

public:
    Model(size_t vertexCount) : graph(vertexCount) {};

    virtual unsigned int calculate() const = 0;
};
