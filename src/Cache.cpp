#include "headers/Cache.hpp"

using namespace std;

Cache::Cache(size_t numSets, size_t numLines, size_t blockSize){
    this->numSets = numSets;
    this->numLinesPerSet = numLines;
    this->blockSize = blockSize;
    this->cacheset_data.resize(numSets, CacheSet(numLines, blockSize));
}

MESIState Cache::getState(unsigned int address){
    //remaining
    return MESIState::I;
}