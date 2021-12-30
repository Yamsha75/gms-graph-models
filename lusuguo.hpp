#pragma once

#define LUSUGUO 1

#include "model.hpp"
#include "graph.hpp"


class LuSuGuo : public Model {
public:
    LuSuGuo(size_t vertexCount);

    unsigned int calculate() const override;
};
