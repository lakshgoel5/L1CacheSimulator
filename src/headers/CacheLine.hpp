#ifndef CACHELINE_HPP
#define CACHELINE_HPP

#include <vector>
#include <iostream>
#include "utils.hpp"
#include <cstdint>

class CacheLines{ //CacheBlock
    private:
    std::vector<int32_t> data; //Each CacheLine contains data
    //Word size is 4 bytes
    MESIState state;
    uint32_t tag; //Every cache line is associated with a tag
    bool valid; //For deciding if data is present in cache or not (initially false)
    // bool dirty;
    uint32_t lru;

    public:
    CacheLines(size_t blockSize);
    void setState(MESIState state);
    MESIState getState();

    void setTag(uint32_t tag);
    uint32_t getTag();

    bool isValid();
    void setValid(bool valid);

    void setLru(uint32_t lru);
    uint32_t getLru();

    void writeData(size_t offset, int32_t data);
    int32_t readData(size_t offset);

    void printCacheLine(); //debug
};

#endif //CACHELINE_HPP