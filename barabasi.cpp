#include <stdio.h>
#include <queue>

#include "barabasi.hpp"


Barabasi::Barabasi(size_t iterations, size_t vertexCount) : Model(vertexCount), iterations(iterations), ternary(vertexCount) {
    // step 0
    graph.addVertex();

    // step 1
    if (iterations >= 1) {
        graph.addVertex({ 0 });
        graph.addVertex({ 0 });
    }

    // further steps
    for (unsigned short int step = 2; step <= iterations; step++) {
        const Graph gc = Graph(graph);

        for (unsigned int _ = 0; _ < 2; _++) {
            size_t offset = graph.merge(gc);

            for (auto const& v : gc.getVertexNeighbours(0))
                if (gc.getVertexNeighbours(v).size() == step - 1)
                    graph.addEdge(0, v + offset);
        }
    }

    // precalculate base3 representation for vertex ids, skipping those with oldest base3 digit == 2
    // (last third of vertices)
    size_t max = vertexCount / 3 * 2;

    for (size_t v = 0; v < max; v++)
        ternary[v] = toTernary(v);
}

Barabasi::Ternary Barabasi::toTernary(size_t v) const {
    Ternary result = Ternary(iterations);

    for (size_t i = iterations - 1; i < iterations; i--) {
        result[i] = v % 3;
        v /= 3;
    }

    return result;
}

unsigned int Barabasi::calculateDistanceToRoot(size_t v) const {
    unsigned int result = 0;
    bool zero = true;

    for (auto const& d : ternary[v])
        if (d == 0 != zero) {
            result++;
            zero = not zero;
        }

    return result;
}

unsigned int Barabasi::calculateAllDistancesToRoot(size_t min, size_t max) const {
    unsigned int result = 0;

    for (size_t v = min; v < max; v++)
        result += calculateDistanceToRoot(v);

    return result;
}

unsigned int Barabasi::calculateDistanceBetweenVertices(size_t a, size_t b) const {
    size_t i = 0;

    Ternary ta = ternary[a];
    Ternary tb = ternary[b];

    for (; i < iterations; i++)
        if (ta[i] != tb[i])
            break;

    unsigned int result = 0;
    bool azero = true;
    bool bzero = true;

    for (size_t j = i; j < iterations; j++) {
        if (ta[j] == 0 != azero) {
            result++;
            azero = not azero;
        }
        if (tb[j] == 0 != bzero) {
            result++;
            bzero = not bzero;
        }
    }

    return result;
}

unsigned int Barabasi::calculateAllDistancesBetweenVertices(size_t min, size_t max) const {
    unsigned int result = 0;

    for (size_t a = min; a < max; a++)
        for (size_t b = a + 1; b < max; b++)
            result += calculateDistanceBetweenVertices(a, b);

    return result;
}

unsigned int Barabasi::calculate() const {
    if (iterations == 0)
        return 0;

    if (iterations == 1)
        return 2;

    size_t third = graph.len() / 3;

    // sum of shortest distances between root and every vertex with oldest base3 digit == 0
    unsigned int sum00 = calculateAllDistancesToRoot(1, third);

    // sum of shortest distances between pairs of vertices with oldest base3 digit == 0, except root
    unsigned int sum01 = calculateAllDistancesBetweenVertices(1, third);

    // sum of shortest distances between root and every vertex with oldest base3 digit == 1
    unsigned int sum10 = calculateAllDistancesToRoot(third, third * 2);

    return (sum00 + sum01) * 3 + (sum00 + 2 * sum10) * third * 2;
}
