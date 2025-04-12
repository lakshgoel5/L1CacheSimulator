#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP
#include "Cache.hpp"
#include "utils.hpp"

class Processor {
    private:
        int processorID;
        Cache cache;
        int numOfCycles;
        ProcessorState state;
        InstructionType instructionType;
    public:
        Processor(int processorID, size_t numSets, size_t numLines, size_t blockSize, string traceFile);
        void cycle();
        void execute();
        void execute_free(InstructionType instructionType);
        ProcessorState getState();
        bool isDone();
};

#endif // PROCESSOR_HPP