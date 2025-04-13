#include "headers/Cache.hpp"
#include <cmath>

using namespace std;

Cache::Cache(size_t numSets, size_t numLines, size_t blockSize){
    this->numSets = numSets;
    this->numLinesPerSet = numLines;
    this->blockSize = blockSize;
    this->cacheset_data.resize(numSets, CacheSet(numLines, blockSize));

    this-> offsetBits = log2(blockSize);
    this->indexBits = log2(numSets);
    this->tagBits = 32 - (offsetBits + indexBits);
    this->indexMask = (1 << indexBits) - 1;
}

MESIState Cache::getState(unsigned int address){
    //remaining
    return MESIState::I;
}

unsigned int Cache::getIndex(unsigned int address){
    unsigned int index = (address & indexMask) >> offsetBits;
    return index;
}
unsigned int Cache::getTag(unsigned int address){
    unsigned int tag = address >> (offsetBits + indexBits);
    return tag;
}

void Cache::updateCache(unsigned int address, MESIState state, vector<int8_t> data){
    unsigned int index = getIndex(address);
    unsigned int tag = getTag(address);
    cacheset_data[index].updateCacheLine(tag, state, data);
}

void Cache::addCacheLine(unsigned int address, MESIState state, vector<int8_t> data){
    unsigned int index = getIndex(address);
    unsigned int tag = getTag(address);
    cacheset_data[index].addCacheLine(tag, state, data);
}