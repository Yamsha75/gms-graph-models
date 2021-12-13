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

// const Vertex Barabasi::toDecimal(Ternary t, size_t skipDigits) {
//     Vertex result = 0;

//     Vertex m = 1; // multiplier; used instead of math pow
//     for (size_t i = iterations; i > skipDigits; i--) {
//         result += m * t[i - 1];
//         m *= 3;
//     }

//     return result;
// }

Barabasi::Barabasi(size_t iterations) : iterations(iterations), graph((Vertex)pow(3.0f, (float)iterations)) {
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

Distance Barabasi::calculateDistanceToRoot(Vertex v) {
    Distance result = 0;
    bool zero = true;

    for (auto const& d : ternaryVertices[v])
        if (d == 0 != zero) {
            result++;
            zero = not zero;
        }

    return result;
}

Distance Barabasi::calculateDistanceBetweenVertices(Vertex a, Vertex b) {
    size_t i = 0;

    Ternary ta = ternaryVertices[a];
    Ternary tb = ternaryVertices[b];

    while (ta[i] == tb[i]) {
        i++;
    }

    Distance result = 0;
    bool zero = true;

    for (size_t j = i; j < iterations; j++) {
        if (ta[j] == 0 != zero) {
            result++;
            zero = not zero;
        }
    }

    zero = true;

    for (size_t j = i; j < iterations; j++) {
        if (tb[j] == 0 != zero) {
            result++;
            zero = not zero;
        }
    }

    return result;
}

void Barabasi::calculateDistancesFromRoot(DistancesMatrix& distances, Vertex min, Vertex max) {
    for (Vertex v = min; v < max; v++)
        distances[0][v] = calculateDistanceToRoot(v);
}

Distance Barabasi::calculateSumOfDistances() {
    if (iterations == 0)
        return 0;

    if (iterations == 1)
        return 2;

    Vertex third = graph.vertexCount / 3;

    // init a map with distances between every possible pair of vertices
    DistancesMatrix distances = DistancesMatrix(graph.vertexCount);

    for (Vertex a = 0; a < graph.vertexCount; a++)
        distances[a] = std::vector<Distance>(graph.vertexCount);

    // calculate shortest distances between every vertex with oldest base3 digit == 0
    for (Vertex a = 0; a < third; a++)
        for (Vertex b = a + 1; b < third; b++)
            distances[a][b] = calculateDistanceBetweenVertices(a, b);

    // calculate shortest distances from root to each vertex with oldest base3 digit == 1
    calculateDistancesFromRoot(distances, third, third * 2);

    // sum of shortest distances between root and every vertex with oldest base3 digit == 0
    Distance sum00 = 0;
    for (Vertex a = 1; a < third; a++)
        sum00 += distances[0][a];

    // sum of shortest distances between pairs of vertices with oldest base3 digit == 0, except root
    Distance sum01 = graph.calculateSumOfDistances(distances, 1, third);

    // sum of shortest distances between root and every vertex with oldest base3 digit == 1
    Distance sum10 = 0;
    for (Vertex a = third; a < third * 2; a++)
        sum10 += distances[0][a];

    return (sum00 + sum01) * 3 + (sum00 + 2 * sum10) * 2 * third;
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
