#include "w1distrib.h"
#include "network.h"
#include "config.h"
#include <algorithm>

w1distrib::w1distrib() : sizeDistribution(SIZE_EXP_LOWER, SIZE_EXP_UPPER), generator(1)
{}

size_t w1distrib::sample(){
    size_t msgSize = (size_t) pow(10, sizeDistribution(generator));
    msgSize = msgSize - (msgSize%Packet::data_packet_size()); // clean packet sizes :)
    return max((size_t)1, msgSize/Packet::data_packet_size());
}