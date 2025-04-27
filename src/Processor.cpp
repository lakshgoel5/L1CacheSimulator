#include "headers/Processor.hpp"
#include <fstream>
#include <iostream>

bool debug_processor = true; // Set to true for debugging

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
            //debug
            //Memory address is 32-bit. If any address is less than 32 bit, assume remaining MSB to be 0. E.g. the
            //address 0x817b08 is actually 0x00817b08.
            address = std::stoul(line.substr(2), nullptr, 16);
            instructionList.push_back(make_pair(instructionType, address));
        }
        this->total_instructions = instructionList.size();
        file.close();
}

void Processor::cycle() {
    numOfCycles++; // number of cycles of that processsor 
    //update values for stats
    //check state of processor
    if(debug_processor) {
        cout << "Current state of processor " << state << endl;
        //print MESI tag of each cache line
        cout << "^^^^^^Cache MESI States^^^^^^" << endl;
        cache.printCacheMESIStates();
    }
    execute();
    //call execute function or any other if needed
}

void Processor::execute() {
    if(state == ProcessorState::FREE) {
        if(debug_processor) {
            cout << "Processor is in FREE state" << endl;
        }
        // Execute the instruction
        // get the instruction from the vector, one at a time
        pair<InstructionType, unsigned int> pair = instructionList[instructionIndex];
        InstructionType instructionType = pair.first; // load,store
        unsigned int address = pair.second; // address
        if(debug_processor) {
            cout << "Instruction: " << (instructionType == InstructionType::LOAD ? "LOAD" : "STORE") << ", Address: " << hex << address << dec << endl;
        }
        // update instruction type to LOAD or STORE (required in execute_free function)
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
        //increment index of vector of instrcution to next if it's a hit
        //call bus with appropriate singal
        // differentiate here with the special case - write miss S
        if(result == ProcessMESIResult::CACHE_HIT) {
            instructionIndex++;
        }
        //stay at same index if it's a miss
        else if(result == ProcessMESIResult::CACHE_MISS) {
            numMiss++;
            //check if data in other caches(done in other function)
            //If in other cache, take one more cycle

            //If not, do as written below
        }
        //change state to read_memory or write_memory depending on R or W if I have to read from memory or writein memory
        //call bus with approprite signal
        //further processing required

        //If I reach to end of vector of instructions, set state to done
        if(instructionIndex >= instructionList.size()) {
            if(debug_processor) {
                cout << "All instructions executed. Now going to Done State" << endl;
            }
            this->state = ProcessorState::DONE;
        }
    } else if(state == ProcessorState::READ_MEMORY) {
        if(debug_processor) {
            cout << "Processor is stalled in READ_MEMORY state" << endl;
        }
        IdleCycles++;
        // stay here until num of idle cycles = 100

        // if num of idle cycles = 100 then set state to free and read from memory update the cache value

        //If I reach to end of vector of instructions, set state to done
    } else if(state == ProcessorState::WRITE_MEMORY) {
        if(debug_processor) {
            cout << "Processor is stalled in WRITE_MEMORY state" << endl;
        }
        IdleCycles++;
        // stay here until num of idle cycles = 100

        // if num of idle cycles = 100 then set state to free and write in memory update the cache value

        //If I reach to end of vector of instructions, set state to done
    } else if(state == ProcessorState::DONE) {
        if(debug_processor) {
            cout << "Processor is in DONE state" << endl;
        }
        // Finalize the operation
        //break out of the function and return
        
    }
}

// returns -> 
ProcessMESIResult Processor::execute_free(InstructionType instructionType, unsigned int address) {
    if(debug_processor){cout << "Starting free execution, going to MESI" << endl; }
    ProcessMESIResult state = mesiProtocol->read(this->processorID, address, *this-> bus,this->cache);
    if(instructionType == InstructionType::LOAD) { // read
        if(state == ProcessMESIResult::CACHE_MISS) {
            this->state = ProcessorState::READ_MEMORY;
        }
        // call mesi function of load
        // get result which is hit or miss(Other work done by MESI only)

        //if miss, I have sent bus request to read from memory or from other caches, and update cache transition to required status(updateCacheState)
        //so no need to do anything

        // if hit then set state to FREE
        // if not hit, then set state to READ_MEMORY, so that I can stay there for 100 cycles
    } else if(instructionType == InstructionType::STORE) { // write
        if(state == ProcessMESIResult::CACHE_MISS) {
            this->state = ProcessorState::WRITE_MEMORY;
        }
        // call mesi function of store
        // get result which is hit or miss(Other work done by MESI only)
        //if miss, I have sent bus request to read from memory, and update cache transition to required status
        //so no need to do anything

        // if hit then set state to FREE
        // if not hit, then set state to WRITE_MEMORY, so that I can stay there for 100 cycles
    }
    if(debug_processor){cout << "State from MESI in write: " << state << endl; }
    return state;
    
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
    cout<<"Core "<<processorID<<" Statistics:"<<endl;
    cout << "Total Instructions: " << total_instructions << endl;
    cout << "Total Reads: " << numReads << endl;
    cout << "Total Writes: " << numWrites << endl;
    cout << "Total Execution Cycles: " << numOfCycles << endl;
    cout << "Idle Cycles: " << IdleCycles << endl;
    cout << "Cache Misses: " << numMissRate << endl;
    cout << "Cache Miss Rate: " << getMissRate() << endl;
    cout << "Cache Evictions: " << numEvictions << endl;
    cout << "Writebacks: " << numWriteBack << endl;
    cout << "Bus Invalidations: " << numBusInvalidate << endl;
    cout << "Data Traffic (Bytes): " << dataTraffic << endl;
}