#include "headers/CacheSet.hpp"

using namespace std;

CacheSet::CacheSet(size_t numLines, size_t blockSize){
    //Initialize list of cache lines
    this->blockSize = blockSize;
    for(size_t i = 0; i < numLines; i++){
        cachelines_data.emplace_back(CacheLines(blockSize));
    }
}

// debug -> when state is invalid -> you dont have to splice -> to be corrected
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

MESIState CacheSet::getState(uint32_t tag){
    for(auto& cacheline : cachelines_data){
        if(cacheline.isValid() == true && cacheline.getTag() == tag){
            return cacheline.getState();
        }
    }
    return MESIState::I; // Return INVALID state if not found
}

void CacheSet::updateCacheLineState(uint32_t tag, MESIState state){
    for(auto it = cachelines_data.begin(); it != cachelines_data.end(); ++it){
        if(it->isValid() == true && it->getTag() == tag){
            it->setState(state);
            //remove cacheline from that place and bring cachline to front
            if(state != MESIState::I){
                cachelines_data.splice(cachelines_data.begin(), cachelines_data, it);
            }
            return;
        }
    }
}

int CacheSet::addCacheLine(uint32_t tag, MESIState state){
    for (auto it = cachelines_data.begin(); it != cachelines_data.end(); ++it) {
        if (!it->isValid()) {
            it->setState(state);
            it->setValid(true);
            it->setTag(tag);
    
            // Move this element to the front
            cachelines_data.splice(cachelines_data.begin(), cachelines_data, it);
            return 0;
        }
    }

    for (auto it = cachelines_data.begin(); it != cachelines_data.end(); ++it) {
        if (it->getState() == MESIState::I) {
            it->setState(state);
            it->setValid(true);
            it->setTag(tag);
            // this->numEvictions++;
            // Move this element to the front
            cachelines_data.splice(cachelines_data.begin(), cachelines_data, it);
            return 0;
        }
    }
    
    CacheLines& lruCacheLine = cachelines_data.back(); // Get the least recently used cache line
    MESIState lrustate = lruCacheLine.getState();

    cachelines_data.pop_back(); // Remove the least recently used cache line
    //print cache lines data from left(front) to right (back)
    cachelines_data.emplace_front(blockSize); // Add a new cache line at the front
    cachelines_data.front().setState(state);
    cachelines_data.front().setValid(true);
    cachelines_data.front().setTag(tag);
    this->numEvictions++;
    if(lrustate == MESIState::M){
        // Handle eviction of modified block
        // Write back to memory or other cache
        numWriteBack++;
        return 100;
    }
    return 0;
}

void CacheSet::printCacheMESIStates(){
    int i=0;
    for(auto& cacheline : cachelines_data){
        MESIState state = cacheline.getState();
        if(state == MESIState::M){
            cout << " M ";
        }
        else if(state == MESIState::E){
            cout << " E ";
        }
        else if(state == MESIState::S){
            cout << " S ";
        }
        else if(state == MESIState::I){
            cout << " I ";
        }
    }
    cout << " ";
}