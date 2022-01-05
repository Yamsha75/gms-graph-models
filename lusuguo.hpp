#pragma once

#define LUSUGUO 1

#include "model.hpp"


class LuSuGuo : public Model {
public:
    LuSuGuo(size_t vertexCount);

    unsigned int calculate() const override;
};
