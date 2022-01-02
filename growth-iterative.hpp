#pragma once

#define GROWTH_ITERATIVE 3

#include "model.hpp"

class GrowthIterative : public Model {
    typedef std::pair<size_t, size_t> Edge;

private:
    unsigned int sumOfShortestDistances = 0;

    static size_t calculateNumberOfVertices(size_t k, size_t r);

    GrowthIterative(size_t iterations, size_t newVertices, size_t vertexCount);

public:
    GrowthIterative(size_t iterations, size_t newVertices);

    unsigned int calculate() const override;
};
