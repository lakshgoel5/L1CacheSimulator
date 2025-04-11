#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP
#include "Cache.hpp"
#include "utils.hpp"

class Processor {
    private:
        Cache cache;
        int numOfCycles;
        ProcessorState state;
        InstructionType instructionType;
    public:
        Processor(size_t numSets, size_t numLines, size_t blockSize) : cache(numSets, numLines, blockSize), numOfCycles(0) {}
        void cycle();
        void execute();
        void execute_free(InstructionType instructionType);
        ProcessorState getState();
};

#endif // PROCESSOR_HPP