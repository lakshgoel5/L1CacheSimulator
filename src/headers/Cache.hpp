#ifndef CACHE_HPP
#define CACHE_HPP

#include <vector>
#include <iostream>
#include <stdexcept>
#include <cstdint>

using namespace std;

// ./L1simulate -t <tracefile> -s <number of set index bits> -E <number of lines per set> -b <block size in bytes> -o <output file>

enum State{
    M, E, S, I
};

class CacheLines{ //CacheBlock
    private:
    std::vector<int32_t> data; //Each CacheLine contains data
    State state;
    uint32_t tag; //Every cache line is associated with a tag
    bool valid; //For deciding if data is present in cache or not (initially false)
    // bool dirty;
    uint32_t lru;

    public:
    CacheLines(size_t blockSize);
    void setState(State state);
    State getSTate();

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

class CacheSet{
    private:
    std::vector<CacheLines> cachelines_data; //Each CacheSet contains lines

    public:
    CacheSet(size_t numLines, size_t blockSize);
};

class Cache{
    private:
    std::vector<CacheSet> cacheset_data; //Each Cache contains cache sets
    uint32_t numSets;
    uint32_t numLinesPerSet;
    uint32_t blockSize;

    public:
    Cache(size_t numSets, size_t numLines, size_t blockSize);
};


#endif