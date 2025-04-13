#include <vector>
#include "CacheLine.hpp"
#include "utils.hpp"

class CacheSet{
    private:
    std::vector<CacheLines> cachelines_data; //Each CacheSet contains lines/blocks

    public:
    CacheSet(size_t numLines, size_t blockSize);
    void updateCacheLine(uint32_t tag, MESIState state, vector<int8_t> data);
    void addCacheLine(uint32_t tag, MESIState state, vector<int8_t> data);
};