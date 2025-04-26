#include <vector>
#include "CacheLine.hpp"
#include <list>

using namespace std;

class CacheSet{
    private:
    std::list<CacheLines> cachelines_data; //Each CacheSet contains lines/blocks
    //why list?
    //I will always have to traverse this dete structure to find required data block, so O(n) anways lagna tha
    //And I can remove element from middle and place it to start in O(1) time
    uint32_t blockSize;

    public:
    int numWriteBack = 0;
    int numEvictions = 0;
    CacheSet(size_t numLines, size_t blockSize);
    void updateCacheLine(uint32_t tag, MESIState state, vector<int8_t> data);
    int addCacheLine(uint32_t tag, MESIState state);
    MESIState getState(uint32_t tag);
    vector<int8_t> readblock(uint32_t tag);
    void updateCacheLineState(uint32_t tag, MESIState state);
};