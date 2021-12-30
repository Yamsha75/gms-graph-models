#include <math.h>
#include <stdio.h>

#include "lusuguo.hpp"


// LuSuGuo::LuSuGuo(Vertex vertexCount) : graph(vertexCount) {
//     // step 0
//     graph.addVertex();

//     // step 1
//     graph.addVertex({ 0 });

//     if (vertexCount == 2)
//         return;

//     graph.addVertex({ 0, 1 });

//     if (vertexCount == 3)
//         return;

//     std::vector<Vertex> parents = std::vector<Vertex>(vertexCount);
//     for (unsigned short i = 0; i < 3; i++)
//         parents[i] = 0;

//     // further steps
//     Vertex n = 3; // number of vertices already created
//     size_t k = 2; // current step

//     // range of vertices from current step that will have a new pair of vertices attached
//     Vertex min = 1; // first vertex from k-1 for which a new pair will be created
//     Vertex max = 2; // last vertex from k-1 for which a new pair will be created
//     Vertex i = min; // current vertex from k-1 for which a new pair will be created
//     short unsigned int j = 0; // current vertex from new pair (0 or 1)

//     while (n < vertexCount) {
//         // n is ID of the new vertex
//         Vertex p = parents[i];

//         if (j == 0) {
//             graph.addVertex({ p, i });
//             parents.push_back(p);
//             j = 1;
//         }
//         else {
//             graph.addVertex({ p, i, n - 1 });
//             parents.push_back(i);
//             j = 0;
//             i++;

//             if (i > max) {
//                 k++;
//                 min = n / 2;
//                 max = n;
//                 i = min;
//             }
//         }
//         n++;
//     }
// }

LuSuGuo::LuSuGuo(size_t vertexCount) : Model(vertexCount) {
    graph.addVertex();

    for (unsigned long int id = 1; id < vertexCount; id++) {
        int l = (int)std::floor(std::log2(id + 1));
        graph.addVertex();

        if (l > 1) {
            graph.addEdge(id, (unsigned long int)std::floor((id - 1) / 2) % (l - 1));
        }

        graph.addEdge(id, (unsigned long int)std::floor((id + 1) / 2) - 1);

        if (id % 2 == 0) {
            graph.addEdge(id, id - 1);
        }
    }
}

unsigned int LuSuGuo::calculate() const {
    return graph.calculateBFS();
}
