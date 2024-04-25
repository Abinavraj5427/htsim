#ifndef DISTRIB_H
#define DISTRIB_H

#include "network.h"

struct distrib{
    virtual size_t sample()=0;
};
#endif