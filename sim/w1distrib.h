#ifndef W1DISTRIB_H
#define W1DISTRIB_H

#include "distrib.h"
#include <random>


#define SIZE_EXP_LOWER 0
#define SIZE_EXP_UPPER 4.5

struct w1distrib:distrib{
    w1distrib();
    size_t sample();

    // message size distribution
    std::uniform_real_distribution<double> sizeDistribution;
    std::default_random_engine generator;
};
#endif