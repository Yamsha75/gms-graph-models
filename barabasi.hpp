#pragma once

#define BARABASI 0

#include <vector>

#include "model.hpp"


class Barabasi : public Model {
    typedef std::vector<unsigned short int> Ternary;

private:
    const size_t iterations;
    std::vector<Ternary> ternary;

    Barabasi(size_t iterations, size_t vertexCount);

    Ternary toTernary(size_t v) const;

    unsigned int calculateDistanceToRoot(size_t v) const;
    unsigned int calculateAllDistancesToRoot(size_t min, size_t max) const;
    unsigned int calculateDistanceBetweenVertices(size_t a, size_t b) const;
    unsigned int calculateAllDistancesBetweenVertices(size_t min, size_t max) const;

public:
    Barabasi(size_t iterations);

    unsigned int calculate() const override;
};
