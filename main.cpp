#include <cstdio>
#include <cstring>

#include "model.hpp"
#include "barabasi.hpp"
#include "lusuguo.hpp"
#include "growth-iterative.hpp"
#include "dcn.hpp"
#include "kronecker.hpp"


int main(int argc, char* argv[]) {
    unsigned short int modelIndex;
    size_t k; // used by all models
    size_t r; // used by GrowthIterative model
    char s[128]; // used by Kronecker model

    Model* m;

    while (scanf("%hu", &modelIndex) != EOF)
        if (scanf("%zu", &k) != EOF) {
            switch (modelIndex) {
            case BARABASI:
                m = new Barabasi(k);
                break;
            case LUSUGUO:
                m = new LuSuGuo(k);
                break;
            case GROWTH_ITERATIVE:
                if (scanf("%zu", &r) == EOF)
                    continue;
                m = new GrowthIterative(k, r);
                break;
            case DCN_:
                m = new DCN(k);
                break;
            case KRONECKER_:
                if (scanf("%s", s) == EOF)
                    continue;
                m = new Kronecker(k, s);
                break;
            default:
                printf("-1\n");
                continue;
            }

            // m->print();
            printf("%u\n", m->calculate());

            delete m;
        }

    return 0;
}
