#ifndef CACHE_HPP
#define CACHE_HPP

#include <vector>
#include <iostream>
#include <stdexcept>

using namespace std;

// ./L1simulate -t <tracefile> -s <number of set index bits> -E <number of lines per set> -b <block size in bytes> -o <output file>

enum State{
    M, E, S, I
};

class CacheLines{
    private:
    State state;
    unsigned int tag;
    // bool valid;
    // bool dirty;
    // int lru;

    // public:
    // CacheLines() : tag(0), valid(false), dirty(false), lru(0) {}
};

class CacheSet{
    private:
    std::vector<CacheLines> cacheSet; //Each CacheSet contains lines
};

class Cache{
    private:
    std::vector<CacheSet> cache; //Each Cache contains cache sets
};


#endif