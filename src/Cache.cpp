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

// processes address
// computes index and tag 
// address -> cache[index] -> cacheset(traverses) -> gets state
MESIState Cache::getState(unsigned int address){
    unsigned int index = getIndex(address);
    unsigned int tag = getTag(address);
    return cacheset_data[index].getState(tag);
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

vector<int8_t> Cache::readblock(unsigned int address){
    unsigned int index = getIndex(address);
    unsigned int tag = getTag(address);
    return cacheset_data[index].readblock(tag);
}

// index -> for identifying cache set
void Cache::updateCacheState(unsigned int address, MESIState state){
    unsigned int index = getIndex(address);
    unsigned int tag = getTag(address);
    cacheset_data[index].updateCacheLineState(tag, state);
}

int Cache::addCacheLine(unsigned int address, MESIState state){
    unsigned int index = getIndex(address);
    unsigned int tag = getTag(address);
    cacheset_data[index].addCacheLine(tag, state);
}