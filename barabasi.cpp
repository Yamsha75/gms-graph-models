#include <stdio.h>
#include <math.h>
#include <queue>

#include "barabasi.hpp"

Ternary Barabasi::toTernary(Vertex v) {
    auto result = Ternary(iterations);

    for (size_t i = iterations; i > 0; i--) {
        result[i - 1] = v % 3;
        v /= 3;
    }

    return result;
}

const Vertex Barabasi::toDecimal(Ternary t, size_t skipDigits) {
    Vertex result = 0;

    Vertex m = 1; // multiplier; used instead of math pow
    for (size_t i = iterations; i > skipDigits; i--) {
        result += m * t[i - 1];
        m *= 3;
    }

    return result;
}

Barabasi::Barabasi(unsigned short int iterations) : iterations(iterations), graph((Vertex)pow(3.0f, (float)iterations)) {
    // step 0
    graph.addVertex();

    // step 1
    if (iterations >= 1)
        for (unsigned short int _ = 0; _ < 2; _++)
            graph.addVertex({ 0 });

    // further steps
    for (unsigned short int step = 2; step <= iterations; step++) {
        Graph gc = Graph(graph, graph.vertexCount); // copy of graph

        for (unsigned int _ = 0; _ < 2; _++) {
            Vertex offset = graph.mergeGraph(gc);

            for (auto const& v : gc.edges[0])
                if (gc.edges[v].size() == step - 1)
                    graph.addEdge(0, v + offset);
        }
    }

    // calculate ternary values for each vertex
    ternaryVertices = std::vector<Ternary>(graph.vertexCount);

    for (Vertex v = 0; v < graph.vertexCount; v++)
        ternaryVertices[v] = toTernary(v);
}

Barabasi::~Barabasi() {}

Distance Barabasi::calculateShortestPathFromRoot(Vertex f) {
    // uses a directed DFS
    Ternary tf = ternaryVertices[f];

    Vertex v = 0; // current vertex
    Distance d = 0; // distance from current vertex to root

    Vertex c; // candidate vertex

    while (v != f) {
        d++;

        if (d > iterations)
            return graph.inf;

        if (graph.edges[v].find(f) != graph.edges[v].end())
            return d;

        unsigned short int dm = 0; // number of ternary digits matched for candidate

        for (auto const& n : graph.edges[v]) {
            Ternary tn = ternaryVertices[n];

            // compare ternary digits between neighbour and final vertex
            for (unsigned short int i = 0; i < iterations; i++) {
                if (tn[i] != tf[i]) {
                    if (i > dm) {
                        // neighbour has more ternary digits matching than current candidate
                        dm = i;
                        c = n;
                    }
                    break;
                }
            }

        }
        // move to best candidate
        v = c;
    }

    return d;
}

Distance Barabasi::calculateSumOfDistances() {
    // return graph.calculateSumOfDistancesBFS();
    // return graph.calculateSumOfDistancesFW();

    // init a map with distances between every possible pair of vertices
    DistancesMatrix distances = DistancesMatrix(graph.vertexCount);

    for (Vertex a = 0; a < graph.vertexCount; a++) {
        distances[a] = std::vector<Distance>(graph.vertexCount);

        for (Vertex b = a + 1; b < graph.vertexCount; b++) {
            if (graph.areVerticesNeighbours(a, b))
                distances[a][b] = 1;
            else
                distances[a][b] = graph.inf;
        }
    }

    // calculate shortest distances from root to each vertex with oldest base3 digit equal to 0
    // simple BFS, starting at root (0)
    graph.calculateDistancesFromRootBFS(distances, graph.vertexCount);

    // calculate shortest distances for each pair of vertices, skipping if both vertices have
    // an ID with oldest digit in ternary equal to 0
    for (Vertex a = 1; a < graph.vertexCount; a++) {
        Ternary ta = ternaryVertices[a];

        for (Vertex b = a + 1; b < graph.vertexCount; b++) {
            if (distances[a][b] != graph.inf)
                continue;

            Ternary tb = ternaryVertices[b];

            unsigned short int similarDigits = 0;
            for (unsigned short int i = 0; i < iterations; i++)
                if (ta[i] != tb[i]) {
                    similarDigits = i;
                    break;
                }

            if (similarDigits > 0) {
                // case can be simplified to base group, which is precalculated; for example:
                // distance between 2201 (73) and 2210 (75) is the same as between 01 (1) and 10 (3)
                Vertex as = toDecimal(ta, similarDigits);
                Vertex bs = toDecimal(tb, similarDigits);

                if (a != as and b != bs) {
                    distances[a][b] = distances[as][bs];
                    continue;
                }
            }
            // all paths go through root, so we can use a sum of distances to root

            // ensure paths from root to both vertices exist
            // if (distances[0][a] == graph.inf)
            //     distances[0][a] = calculateShortestPathFromRoot(a);

            // if (distances[0][b] == graph.inf)
            //     distances[0][b] = calculateShortestPathFromRoot(b);

            distances[a][b] = distances[0][a] + distances[0][b];
        }
    }

    return graph.calculateSumOfDistances(distances);
}

void Barabasi::print() {
    // print all vertices ternary representations in double quotes
    for (Vertex v = 0; v < graph.vertexCount; v++) {
        printf("\"");
        for (auto const& d : ternaryVertices[v]) {
            printf("%hu", d);
        }
        printf("\"\n");
    }

    // print all edges, using ternary vertices representations in double quotes
    for (Vertex v = 0; v < graph.vertexCount; v++)
        for (Vertex n : graph.edges[v]) {
            printf("\"");
            for (auto const& d : ternaryVertices[v])
                printf("%hu", d);
            printf("\" \"");
            for (auto const& d : ternaryVertices[n])
                printf("%hu", d);
            printf("\"\n");
        }
}
