#ifndef CACHELINE_HPP
#define CACHELINE_HPP

#include <vector>
#include <iostream>
#include "utils.hpp"
#include <cstdint>

class CacheLines{ //CacheBlock
    private:
    std::vector<int8_t> data; //Each CacheLine contains data
    //Word size is 4 bytes
    MESIState state;
    unsigned int tag; //Every cache line is associated with a tag
    bool valid; //For deciding if data is present in cache or not (initially false)
    // bool dirty;
    uint32_t lru;

    public:
    CacheLines(size_t blockSize);
    void setState(MESIState state);
    MESIState getState();

    void setTag(unsigned int tag);
    unsigned int getTag();

    bool isValid();
    void setValid(bool valid);

    void setLru(uint32_t lru);
    uint32_t getLru();

    void writebyte(size_t offset, int32_t data);
    void writeblock(vector<int8_t> data);
    int8_t readbyte(size_t offset);
    int readword(size_t offset);


    void printCacheLine(); //debug
};

#endif //CACHELINE_HPP