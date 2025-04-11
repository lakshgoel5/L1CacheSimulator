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

enum State{ //change name
    M, E, S, I
};

enum ProcessorState {
    FREE,
    READ_MEMORY,
    WRITE_MEMORY,
    DONE
};

enum InstructionType {
    LOAD,
    STORE,
};
#endif // UTILS_HPP