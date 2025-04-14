#ifndef UTILS_HPP
#define UTILS_HPP

enum ProcessMESIResult{
    CACHE_HIT,
    CACHE_MISS
};

enum BusTransaction{
    MEMREAD,
    INVALIDATE,
    RWITM
};

enum MESIState{ //change name
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

enum TransactionType {
    BUSRD,
    BUSRDX
};
#endif // UTILS_HPP