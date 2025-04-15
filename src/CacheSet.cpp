#include "headers/CacheSet.hpp"

using namespace std;

CacheSet::CacheSet(size_t numLines, size_t blockSize){
    //Initialize list of cache lines
    this->blockSize = blockSize;
    for(size_t i = 0; i < numLines; i++){
        cachelines_data.emplace_back(blockSize);
    }
}

void CacheSet::updateCacheLine(uint32_t tag, MESIState state, vector<int8_t> data){ //also used for adding cache line
    for(auto it = this->cachelines_data.begin(); it != this->cachelines_data.end(); ++it){
        if(it->isValid() == true && it->getTag() == tag){
            it->setState(state);
            it->setValid(true);
            it->setTag(tag);
            it->writeblock(data);
            cachelines_data.splice(cachelines_data.begin(), cachelines_data, it); // Move to front
            return;
        }
    }
    throw std::runtime_error("Cache set not found");
}

void CacheSet::addCacheLine(uint32_t tag, MESIState state, vector<int8_t> data){
    for(auto it = this->cachelines_data.begin(); it != this->cachelines_data.end(); ++it){
        if(it->isValid() == false){
            it->setState(state);
            it->setValid(true);
            it->setTag(tag);
            it->writeblock(data);
            cachelines_data.splice(cachelines_data.begin(), cachelines_data, it); // Move to front
            return;
        }
    }
    throw std::runtime_error("Cache set is full");
}

MESIState CacheSet::getState(uint32_t tag){
    for(auto& cacheline : cachelines_data){
        if(cacheline.isValid() == true && cacheline.getTag() == tag){
            return cacheline.getState();
        }
    }
    return MESIState::I; // Return INVALID state if not found
}

vector<int8_t> CacheSet::readblock(uint32_t tag){
    for(auto& cacheline : cachelines_data){
        if(cacheline.isValid() == true && cacheline.getTag() == tag){
            return cacheline.readblock();
        }
    }
    throw std::runtime_error("Cache line not found");
}

void CacheSet::updateCacheLineState(uint32_t tag, MESIState state){
    for(auto& cacheline : cachelines_data){
        if(cacheline.isValid() == true && cacheline.getTag() == tag){
            cacheline.setState(state);
            return;
        }
    }
}

void CacheSet::addCacheLine(uint32_t tag, MESIState state, vector<int8_t> data){
    for(auto& cacheline : cachelines_data){
        if(cacheline.isValid() == false){
            cacheline.setState(state);
            cacheline.setValid(true);
            cacheline.setTag(tag);
            cacheline.writeblock(data);
            return;
        }
    }
    cachelines_data.pop_back(); // Remove the least recently used cache line
    cachelines_data.emplace_front(blockSize); // Add a new cache line at the front
}