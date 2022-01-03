#pragma once

#define KRONECKER_ 6

#include "model.hpp"


class Kronecker : public Model {
private:
    size_t baseVertexCount;

    Kronecker(size_t iterations, const char* adjacencyMatrix, size_t baseVertexCount);

public:
    Kronecker(size_t iterations, const char* adjacencyMatrix);

    unsigned int calculate() const override;
};
