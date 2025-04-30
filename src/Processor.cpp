#include "headers/Processor.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>

bool debug_processor = false; // Set to true for debugging

Processor::Processor(int processorID, size_t numSets, size_t numLines, size_t blockSize, string traceFile, Bus* bus)
    : processorID(processorID), numOfCycles(0), state(ProcessorState::FREE), cache(numSets, numLines, blockSize) {
        instructionIndex = 0;
        // read trace file
        ifstream file(traceFile);
        this-> bus = bus;
        if (!file.is_open()) {
            throw std::runtime_error("Could not open trace file");
        }

        string line;
        while (getline(file, line)) {
            // Parse the instruction and address from the line
            InstructionType instructionType;
            unsigned int address;
            if (line[0] == 'R' || line[0] == 'r') {
                this -> numReads++;
                instructionType = InstructionType::LOAD;
            } else if (line[0] == 'W' || line[0] == 'w') {
                this -> numWrites++;
                instructionType = InstructionType::STORE;
            } else {
                throw std::runtime_error("Invalid instruction type in trace file");
            }
            //Memory address is 32-bit. If any address is less than 32 bit, assume remaining MSB to be 0. E.g. the
            //address 0x817b08 is actually 0x00817b08.
            address = std::stoul(line.substr(2), nullptr, 16);
            instructionList.push_back(make_pair(instructionType, address));
        }
        this->total_instructions = instructionList.size();
        file.close();
}

void Processor::cycle() {

    if(instructionIndex >= instructionList.size()) {
        if(debug_processor) {cout << "All instructions executed. Now going to Done State" << endl;}
        this->state = ProcessorState::DONE;
        return;
    }
    numOfCycles++; // number of cycles of that processsor
    if(debug_processor) {cout << "Current state of processor " << state << endl;cout << "^^^^^^Cache MESI States^^^^^^" << endl;}
    execute();
}

void Processor::execute() {
    if(state == ProcessorState::FREE) {
        if(debug_processor) {cout << "Processor is in FREE state" << endl;}
        // Execute the instruction
        // get the instruction from the vector, one at a time


        pair<InstructionType, unsigned int> pair = instructionList[instructionIndex];
        InstructionType instructionType = pair.first; // load,store
        unsigned int address = pair.second; // address


        if(debug_processor) {cout << "Instruction: " << (instructionType == InstructionType::LOAD ? "LOAD" : "STORE") << ", Address: " << hex << address << dec << endl;}

        // call execute_free function
        ProcessMESIResult result = execute_free(instructionType, address);


        if(debug_processor) {
            cout << "Result from execute_free: ";
            if(result == ProcessMESIResult::CACHE_HIT) {
                cout << "CACHE_HIT" << endl;
            } else if(result == ProcessMESIResult::CACHE_MISS) {
                cout << "CACHE_MISS" << endl;
            }
        }

        //mark
        //increment index of vector of instrcution to next if it's a hit
        // differentiate here with the special case - write miss S
        if(result == ProcessMESIResult::CACHE_HIT) {
            instructionIndex++;
        }
        //stay at same index if it's a miss
        else if(result == ProcessMESIResult::CACHE_MISS) {
            //in this case, instructionIndex will not be incremented after the bus request is done (inside function update state to free)
            numMiss++;
        }
        //change state to read_memory or write_memory done in execute_free

    } else if(state == ProcessorState::READ_MEMORY) {
        if(debug_processor) {cout << "Processor is stalled in READ_MEMORY state" << endl;}
        IdleCycles++;
        //stays here until it is reading
    } else if(state == ProcessorState::WRITE_MEMORY) {
        if(debug_processor) {cout << "Processor is stalled in WRITE_MEMORY state" << endl;}
        IdleCycles++;
        // stay here until it is writing back
    } else if(state == ProcessorState::DONE) {
        if(debug_processor) {cout << "Processor is in DONE state" << endl;}
        // Finalize the operation
    }
}

/// @brief calls the mesi protocol, which decides a hit or miss
/// @param instructionType 
/// @param address 
/// @return return if it's a hit or miss
ProcessMESIResult Processor::execute_free(InstructionType instructionType, unsigned int address) {
    if(debug_processor){cout << "Starting free execution, going to MESI" << endl; }

    ProcessMESIResult state1;

    if(instructionType == InstructionType::LOAD) { // read
        // call mesi function of load
        // get result which is hit or miss(Other work done by MESI only)
        state1 = mesiProtocol->read(this->processorID, address, *this-> bus,this->cache);

        //if miss, then FREE -> READ_MEMORY
        if(state1 == ProcessMESIResult::CACHE_MISS) {
            this->state = ProcessorState::READ_MEMORY;
        }

    } else if(instructionType == InstructionType::STORE) { // write
        // call mesi function of store
        // get result which is hit or miss(Other work done by MESI only)
        state1 = mesiProtocol->write(this->processorID, address, *this-> bus,this->cache);

        //if miss, then FREE -> WRITE_MEMORY
        if(state1 == ProcessMESIResult::CACHE_MISS) {
            this->state = ProcessorState::WRITE_MEMORY;
        }
    }
    return state1;
}

ProcessorState Processor::getState() {
    return state;
}

bool Processor::isDone() {
    return (state == ProcessorState::DONE);
}

MESIState Processor::getCacheState(unsigned int address) {
    return cache.getState(address);
}

void Processor::updatecacheState(unsigned int address, MESIState state) {
    cache.updateCacheState(address, state);
}

int Processor::addCacheLine(unsigned int address, MESIState state) {
    return this->cache.addCacheLine(address, state);
}

void Processor::PrintStats() {
    numEvictions = this->cache.getnumEvictions();
    cout<<"Core "<<processorID<<" Statistics:"<<endl;
    cout << "Total Instructions: " << total_instructions << endl;
    cout << "Total Reads: " << numReads << endl;
    cout << "Total Writes: " << numWrites << endl;
    cout << "Total Execution Cycles: " << numOfCycles << endl;
    cout << "Idle Cycles: " << IdleCycles << endl;
    cout << "Cache Misses: " << numMiss << endl;
    cout << "Cache Miss Rate: " << std::setprecision(3) << getMissRate() <<"%"<< endl;
    cout << "Cache Evictions: " << numEvictions << endl;
    cout << "Writebacks: " << getNumWriteBack() << endl;
    cout << "Bus Invalidations: " << numBusInvalidate << endl;
    cout << "Data Traffic (Bytes): " << dataTraffic << endl;
}