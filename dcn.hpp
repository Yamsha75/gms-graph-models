#pragma once

#define DCN_ 4

#include "model.hpp"


class DCN : public Model {
private:
    static size_t getVertexParent(size_t v);

public:
    DCN(size_t vertexCount);

    unsigned int calculate() const override;
};
