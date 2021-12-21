#pragma once

#include "graph.hpp"
#include "am_graph.hpp"


#define LUSUGUO 1


class LuSuGuo {
public:
    AMGraph graph;

    LuSuGuo(Vertex vertexCount);

    // calculate the sum of shortest distances in graph
    Distance calculateSumOfShortestDistances() const;
};
