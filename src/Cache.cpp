#include "headers/Cache.hpp"

CacheLines::CacheLines(size_t blockSize){
    data.resize(blockSize);
    state = I; //Initial state is invalid
    tag = 0; //Initial tag is 0
    valid = false; //Initially not valid
    lru = 0;
}

void CacheLines::setState(MESIState state){
    this->state = state;
}

MESIState CacheLines::getState(){
    return state;
}

void CacheLines::setTag(uint32_t tag){
    this->tag = tag;
}
uint32_t CacheLines::getTag(){
    return tag;
}

bool CacheLines::isValid(){
    return valid;
}
void CacheLines::setValid(bool valid){
    this->valid = valid;
}

void CacheLines::setLru(uint32_t lru){
    this->lru = lru;
}
uint32_t CacheLines::getLru(){
    return lru;
}

void CacheLines::writeData(size_t offset, int32_t value){
    if(offset >= this->data.size()){
        throw out_of_range("index out of range: " + to_string(offset));
    }
    this->data[offset] = value;
}
int32_t CacheLines::readData(size_t offset){
    if(offset >= this->data.size()){
        throw out_of_range("index out of range: " + to_string(offset));
    }
    return data[offset];
}


CacheSet::CacheSet(size_t numLines, size_t blockSize){
    this->cachelines_data.resize(numLines, CacheLines(blockSize));
}

Cache::Cache(size_t numSets, size_t numLines, size_t blockSize){
    this->numSets = numSets;
    this->numLinesPerSet = numLines;
    this->blockSize = blockSize;
    this->cacheset_data.resize(numSets, CacheSet(numLines, blockSize));
}