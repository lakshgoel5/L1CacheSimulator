#ifndef CACHE_HPP
#define CACHE_HPP

#include <vector>
#include <iostream>
#include <stdexcept>
#include <cstdint>
#include "utils.hpp"
#include "CacheSet.hpp"

using namespace std;

// ./L1simulate -t <tracefile> -s <number of set index bits> -E <number of lines per set> -b <block size in bytes> -o <output file>


class Cache{
    private:
    std::vector<CacheSet> cacheset_data; //Each Cache contains cache sets
    uint32_t numSets;
    uint32_t numLinesPerSet;
    uint32_t blockSize;

    unsigned int indexMask;
    unsigned int offsetBits;
    unsigned int indexBits;
    unsigned int tagBits;

    public:
    Cache(size_t numSets, size_t numLines, size_t blockSize);
    MESIState getState(unsigned int address);
    unsigned int getIndex(unsigned int address);
    unsigned int getTag(unsigned int address);
    void updateCache(unsigned int address, MESIState state, vector<int8_t> data);
    int addCacheLine(unsigned int address, MESIState state);
    vector<int8_t> readblock(unsigned int address);
    void updateCacheState(unsigned int address, MESIState state);
    int getBlockSize(){
        return this->blockSize;
    }
};


#endif