#include "barabasi.hpp"

Graph barabasi(unsigned int steps) {
    Graph g = Graph();

    // step 0
    g.addVertex();

    // further steps
    for (unsigned int step = 1; step <= steps; step++) {
        Graph gc = Graph(g);

        for (unsigned int _ = 0; _ < 2; _++) {
            Vertex offset = g.mergeGraph(gc);

            if (gc.getEdges().find(0)->second.size() == 0)
                g.addEdge(0, offset);

            for (auto const& pair : gc.getEdges()) {
                if (gc.areVerticesNeighbours(0, pair.first)) {
                    Vertex neighboursCount = pair.second.size();

                    if (neighboursCount == step - 1)
                        g.addEdge(0, pair.first + offset);
                }
            }
        }
    }

    return g;
}
