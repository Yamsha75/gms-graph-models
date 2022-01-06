#pragma once

#include "graph.hpp"


class Model {
protected:
    Graph* graph;

public:
    ~Model() { delete graph; };

    virtual unsigned int calculate() const = 0;

    void printGraph() { graph->printEdges(); };
};
