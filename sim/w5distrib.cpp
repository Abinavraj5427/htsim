#include "w5distrib.h"
#include <cassert>
#include <fstream>

std::vector<size_t> w5distrib::sizes = std::vector<size_t>(400);
std::vector<double> w5distrib::probs = std::vector<double>(400);

void w5distrib::fill_distrib(){
    std::ifstream is;
    is.open(DISTRIB_PATH);
    for(size_t i = 0; i < 400; i++){
        is >> sizes[i];
        is >> probs[i];
    }
    is.close();
}

// sample from distribution
size_t w5distrib::sample(){
    double r = ((double)rand()) / RAND_MAX;
    size_t size;
    for(size_t i = 0;i < 400; i++){
        if(r <= probs[i]){
            return max((size_t) 1, sizes[i]*1500/Packet::data_packet_size());
        }
    }
    return max((size_t) 1, sizes[sizes.size()-1]*1500/Packet::data_packet_size());
}