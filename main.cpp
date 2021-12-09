#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>

#include "graph.hpp"

#include "barabasi.hpp"

// #define DEBUG

void calculateGraph(unsigned int modelIndex, unsigned long modelParameters) {
    Graph g;

#ifdef DEBUG
    printf("calculating for %u, %lu\n", modelIndex, modelParameters);
#endif

    switch (modelIndex) {
    case BARABASI: {
        Barabasi b = Barabasi(modelParameters);

        printf("%lu\n", b.calculateSumOfDistances());

        break;
    }
    default:
        printf("-1\n");
        return;
    }
}

int main(int argc, char* argv[]) {
    unsigned int modelIndex;
    unsigned long modelParameters;

    if (argc > 1) {
        // parameters given with program execution
        if (argc % 2 == 1) {
            for (unsigned int i = 1; i < argc; i += 2) {
                if (sscanf(argv[i], "%u", &modelIndex) == 1 and sscanf(argv[i + 1], "%lu", &modelParameters) == 1)
                    calculateGraph(modelIndex, modelParameters);
                else {
                    printf("unexpected argument pair: %s %s! must be two non-negative integers!\n", argv[i], argv[i + 1]);
                }
            }
        }
        else {
            printf("unexpected number of arguments: %d! must be an even number!\n", argc);
            return 1;
        }
    }
    else {
        // await parameters from stdin
        while (scanf("%u %lu", &modelIndex, &modelParameters) == 2)
            calculateGraph(modelIndex, modelParameters);
    }

    return 0;
}
