#include <vector>
#include "CacheLine.hpp"
#include "utils.hpp"

class CacheSet{
    private:
    std::vector<CacheLines> cachelines_data; //Each CacheSet contains lines

    public:
    CacheSet(size_t numLines, size_t blockSize);
};