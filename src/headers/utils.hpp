#ifndef UTILS_HPP
#define UTILS_HPP

enum ProcessResult{
    CACHE_HIT,
    CACHE_MISS
};

enum BusTransaction{
    MEMREAD,
    INVALIDATE,
    RWITM
};
#endif // UTILS_HPP