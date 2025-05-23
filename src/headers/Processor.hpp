#pragma once
#include "Cache.hpp"
#include "utils.hpp"
#include "mesi.hpp"
#include "Bus.hpp"

class Bus; // Forward declaration
class MESIProtocol; // Forward declaration

class Processor {
    private:
        int processorID;
        Cache cache;
        MESIProtocol* mesiProtocol;
        Bus* bus;
        ProcessorState state;
        InstructionType instructionType;
        vector<pair<InstructionType, unsigned int>> instructionList; // Vector of instructions (LOAD/STORE, address) 
        int instructionIndex;
    public:
        bool halted = false;
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
        long long dataTraffic = 0;
        Processor(int processorID, size_t numSets, size_t numLines, size_t blockSize, string traceFile, Bus* bus);
        void cycle();
        void execute();
        ProcessMESIResult execute_free(InstructionType instructionType, unsigned int address);
        ProcessorState getState();
        MESIState getCacheState(unsigned int address);
        bool isDone();
        void updatecacheState(unsigned int address, MESIState state);
        int addCacheLine(unsigned int address, MESIState state);
        int getBlockSize(){
            return cache.getBlockSize();
        }
        float getMissRate(){
            return ((float)numMiss/(float)total_instructions)*100;
        }
        void PrintStats();
        void updateStateToFree() {
            if(this->state != ProcessorState::DONE){
                this->state = ProcessorState::FREE;
                this->instructionIndex++;
            }
        }
        void printCache() {
            cache.printCacheMESIStates();
        }
        long getNumWriteBack() {
            return this->numWriteBack + this->cache.getnumWriteBack();
        }
};