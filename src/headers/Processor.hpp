#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP
#include "Cache.hpp"
#include "utils.hpp"
#include "mesi.hpp"
#include "Bus.hpp"

class Processor {
    private:
        int processorID;
        Cache cache;
        int numOfCycles = 0;
        int total_instructions = 0;
        int numReads = 0;
        int numWrites = 0;
        int IdleCycles = 0;
        int numMiss = 0;
        int numMissRate = 0; // will be computed at last
        int numEvictions = 0;
        int numWriteBack = 0;
        int numBusInvalidate = 0;
        int dataTraffic = 0;
        MESIProtocol MESIProtocol;
        Bus* bus;
        ProcessorState state;
        InstructionType instructionType;
        vector<pair<InstructionType, unsigned int>> instructionList; // Vector of instructions (LOAD/STORE, address) 
        int instructionIndex;
    public:
        Processor(int processorID, size_t numSets, size_t numLines, size_t blockSize, string traceFile, Bus* bus);
        void cycle();
        void execute(ProcessorState state);
        ProcessMESIResult execute_free(InstructionType instructionType, unsigned int address);
        ProcessorState getState();
        MESIState getCacheState(unsigned int address);
        bool isDone();
        void updatecacheState(unsigned int address, MESIState state);
        int addCacheLine(unsigned int address, MESIState state);
        int getBlockSize(){
            return cache.getBlockSize();
        }
};

#endif // PROCESSOR_HPP