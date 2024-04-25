#ifndef W5DISTRIB_H
#define W5DISTRIB_H

#include "distrib.h"
#include <vector>

#define DISTRIB_PATH "../w5distrib.txt"
struct w5distrib{
    static std::vector<size_t> sizes;
    static std::vector<double> probs;
    static size_t sample();
    static void fill_distrib();
};

#endif