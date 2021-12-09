#include "barabasi.hpp"

#include <queue>

ternary Barabasi::toTernary(vertex v) {
    auto result = ternary(iterations);

    for (int i = iterations - 1; i >= 0; i--) {
        result[i] = v % 3;
        v /= 3;
    }

    return result;
}

const vertex Barabasi::toDecimal(ternary t, unsigned short int skipDigits) {
    vertex v = 0;

    vertex m = 1;
    // for (unsigned short int i = 0; i < iterations - skipDigits; i++) {
    //     v += m * t[iterations - (i + 1)];
    //     m *= 3;
    // }
    for (short int i = iterations - 1; i >= skipDigits; i--) {
        v += m * t[i];
        m *= 3;
    }

    return v;
}

Barabasi::Barabasi(unsigned short int iterations) : iterations(iterations) {
    // generate graph
    graph = Graph();

    // step 0
    graph.addVertex();

    // further steps
    for (unsigned int step = 1; step <= iterations; step++) {
        Graph gc = Graph(graph); // copy of graph

        for (unsigned int _ = 0; _ < 2; _++) {
            vertex offset = graph.mergeGraph(gc);

            if (gc.edges[0].size() == 0)
                graph.addEdge(0, offset);

            for (auto const& pair : gc.edges) {
                if (gc.areVerticesNeighbours(0, pair.first)) {
                    vertex neighboursCount = pair.second.size();

                    if (neighboursCount == step - 1)
                        graph.addEdge(0, pair.first + offset);
                }
            }
        }
    }

    // calculate ternary values for each vertex
    for (vertex v = 0; v < graph.vertexCount; v++)
        ternaryVertices[v] = toTernary(v);
}

Barabasi::~Barabasi() {}

distance Barabasi::calculateShortestPathFromRoot(vertex f) {
    // uses a directed DFS
    ternary tf = ternaryVertices[f];

    vertex v = 0; // current vertex
    distance d = 0; // distance from current vertex to root

    vertex c; // candidate vertex
    unsigned short int dm; // number of ternary digits matched for candidate

    while (v != f) {
        d++;

        if (d > iterations)
            return graph.inf;

        if (graph.edges[v].find(f) != graph.edges[v].end())
            return d;

        dm = 0;

        for (auto const& n : graph.edges[v]) {
            ternary tn = ternaryVertices[n];

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

distance Barabasi::calculateSumOfDistances() {
    // init a map with distances between every possible pair of vertices
    std::map<vertex, std::map<vertex, distance>> distances = {};

    for (vertex v = 0; v < graph.vertexCount; v++) {
        for (vertex o = v + 1; o < graph.vertexCount; o++) {
            if (graph.areVerticesNeighbours(v, o))
                distances[v][o] = 1;
            else
                distances[v][o] = graph.inf;
        }
    }

    // calculate shortest distances from root to each vertex with oldest base3 digit equal to 0
    // simple BFS, starting at root (0)
    graph.calculateDistancesToRootBFS(distances, graph.vertexCount / 3);

    // calculate shortest distances for each pair of vertices, skipping if both vertices have
    // an ID with oldest digit in ternary equal to 0
    for (vertex a = 0; a < graph.vertexCount; a++) {
        ternary ta = ternaryVertices[a];

        for (vertex b = a + 1; b < graph.vertexCount; b++) {
            if (distances[a][b] != graph.inf)
                continue;

            ternary tb = ternaryVertices[b];

            unsigned short int similarDigits = 0;
            for (unsigned short int i = 0; i < iterations; i++)
                if (ta[i] != tb[i]) {
                    similarDigits = i;
                    break;
                }

            if (similarDigits > 0) {
                // case can be simplified to base group, which is precalculated; for example:
                // distance between 2201 (73) and 2210 (75) is the same as between 01 (1) and 10 (3)
                vertex as = toDecimal(ta, similarDigits);
                vertex bs = toDecimal(tb, similarDigits);

                if (a != as and b != bs) {
                    distances[a][b] = distances[as][bs];
                    continue;
                }
            }
            // all paths go through root, so we can use a sum of distances to root

            // ensure paths from root to both vertices exist
            if (distances[0][a] == graph.inf)
                distances[0][a] = calculateShortestPathFromRoot(a);

            if (distances[0][b] == graph.inf)
                distances[0][b] = calculateShortestPathFromRoot(b);

            distances[a][b] = distances[0][a] + distances[0][b];
        }
    }

    return graph.calculateSumOfDistances(distances);
}

void Barabasi::print() {
    // print all vertices ternary representations in double quotes
    for (auto const& pair : ternaryVertices) {
        printf("\"");
        for (auto const& x : pair.second) {
            printf("%hu", x);
        }
        printf("\"\n");
    }

    // print all edges, using ternary vertices representations in double quotes
    for (auto const& pair : graph.edges)
        for (auto const& o : pair.second)
            if (pair.first < o) {
                printf("\"");
                for (auto const& d : ternaryVertices[pair.first]) {
                    printf("%hu", d);
                }
                printf("\" \"");
                for (auto const& d : ternaryVertices[o]) {
                    printf("%hu", d);
                }
                printf("\"\n");
            }
}
