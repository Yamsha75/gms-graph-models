#include <cmath>
#include <cstdio>

#include "lusuguo.hpp"
#include "lst_graph.hpp"


LuSuGuo::LuSuGuo(size_t vertexCount) {
    graph = new ListGraph(vertexCount);

    graph->addVertex();

    for (size_t id = 1; id < vertexCount; id++) {
        int l = (int)std::floor(std::log2(id + 1));
        graph->addVertex();

        if (l > 1)
            graph->addEdge(id, (size_t)std::floor((id - 1) / 2) % (l - 1));

        graph->addEdge(id, (size_t)std::floor((id + 1) / 2) - 1);

        if (id % 2 == 0)
            graph->addEdge(id, id - 1);
    }
}

unsigned int LuSuGuo::calculate() const {
    return graph->calculateBFS();
}
