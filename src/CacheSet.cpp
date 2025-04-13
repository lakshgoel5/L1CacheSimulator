#include "headers/CacheSet.hpp"

using namespace std;

CacheSet::CacheSet(size_t numLines, size_t blockSize){
    this->cachelines_data.resize(numLines, CacheLines(blockSize));
}

void CacheSet::updateCacheLine(uint32_t tag, MESIState state, vector<int8_t> data){ //also used for adding cache line
    for(auto& cacheline : cachelines_data){
        if(cacheline.isValid() == true && cacheline.getTag() == tag){ //debug
            cacheline.setState(state);
            cacheline.setValid(true);
            cacheline.setTag(tag);
            cacheline.setLru(0); // Reset LRU value //debug change lru order in cache
            cacheline.writeblock(data);
            return;
        }
    }
    throw std::runtime_error("Cache set not found");
}

void CacheSet::addCacheLine(uint32_t tag, MESIState state, vector<int8_t> data){
    for(auto& cacheline : cachelines_data){
        if(cacheline.isValid() == false){
            cacheline.setState(state);
            cacheline.setValid(true);
            cacheline.setTag(tag);
            cacheline.setLru(0); // Reset LRU value//debug change lru order in cache
            cacheline.writeblock(data);
            return;
        }
    }
    throw std::runtime_error("Cache set is full");
}