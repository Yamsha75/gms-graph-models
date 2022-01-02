#include <stdio.h>
#include <sstream>
#include <iostream>
#include <string>
#include <iterator>

#include "model.hpp"
#include "barabasi.hpp"
#include "lusuguo.hpp"
#include "growth-iterative.hpp"
#include "dcn.hpp"


void calculateModel(unsigned short int modelIndex, size_t k, size_t r = 0) {
    Model* m;

    switch (modelIndex) {
    case BARABASI:
        m = new Barabasi(k);
        break;
    case LUSUGUO:
        m = new LuSuGuo(k);
        break;
    case GROWTH_ITERATIVE:
        m = new GrowthIterative(k, r);
        break;
    case DCN_:
        m = new DCN(k);
        break;
    default:
        printf("-1\n");
        return;
    }

    printf("%u\n", m->calculate());

    delete m;
}

int main(int argc, char* argv[]) {
    unsigned short int modelIndex;
    size_t k, r;

    while (scanf("%hu", &modelIndex) != EOF)
        if (scanf("%zu", &k) != EOF) {
            switch (modelIndex) {
            case GROWTH_ITERATIVE:
                if (scanf("%zu", &r) == EOF)
                    continue;
            default:
                calculateModel(modelIndex, k, r);
            }
        }

    return 0;
}
