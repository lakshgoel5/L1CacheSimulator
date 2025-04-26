#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP
#include "Cache.hpp"
#include "utils.hpp"

class Processor {
    private:
        int processorID;
        Cache cache;
        int numOfCycles;
        int numInstr;
        int numReads;
        int numWrites;
        int numCycles;
        int numIdleCycles;
        int numMiss;
        int numMissRate; // will be computed at last
        int numEvictions;
        int numWriteBack;
        int numBusInvalidate;
        int dataTraffic;
        ProcessorState state;
        InstructionType instructionType;
        vector<pair<InstructionType, unsigned int>> instructionList; // Vector of instructions (LOAD/STORE, address) 
        int instructionIndex;
    public:
        Processor(int processorID, size_t numSets, size_t numLines, size_t blockSize, string traceFile);
        void cycle();
        void execute(ProcessorState state);
        ProcessMESIResult execute_free(InstructionType instructionType);
        ProcessorState getState();
        MESIState getCacheState(unsigned int address);
        bool isDone();
        void updatecacheState(unsigned int address, MESIState state);
};

#endif // PROCESSOR_HPP