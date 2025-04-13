#include "headers/CacheSet.hpp"

using namespace std;

CacheSet::CacheSet(size_t numLines, size_t blockSize){
    this->cachelines_data.resize(numLines, CacheLines(blockSize));
}