#include "headers/Processor.hpp"

Processor::Processor(int processorID, size_t numSets, size_t numLines, size_t blockSize, string traceFile)
    : processorID(processorID), numOfCycles(0), state(ProcessorState::FREE), cache(numSets, numLines, blockSize) {
        // read trace file
        //save data in a vector, make it in .hpp file
}

void Processor::cycle() {
    numOfCycles++;
    //update values for stats
    // get the instruction from the vector, one at a time
    //call execute function or any other if needed
}

void Processor::execute() {
    if(state == ProcessorState::FREE) {
        // Execute the instruction
        //update instruction type
        // call execute_free function

        //increment index of vector of instrcution to next if it's a hit
        //call bus with appropriate singal
        
        //stay at same index if it's a miss
        //change state to read_memory or write_memory depending on R or W
        //call bus with approprite signal
        //further processing required

        //If I reach to end of vector of instructions, set state to done
    } else if(state == ProcessorState::READ_MEMORY) {
        // stay here until num of idle cycles = num of cycles to read from memory

        // if num of idle cycles = num of cycles to read from memory then set state to free
        // update the cache value(don't know)

        //If I reach to end of vector of instructions, set state to done
    } else if(state == ProcessorState::WRITE_MEMORY) {
        // stay here until num of idle cycles = num of cycles to read from memory

        // if num of idle cycles = num of cycles to read from memory then set state to free
        // update the cache value(don't know)

        //If I reach to end of vector of instructions, set state to done
    } else if(state == ProcessorState::DONE) {
        // Finalize the operation
        //break out of the function and return
        
    }
}

void Processor::execute_free(InstructionType instructionType) {
    if(instructionType == InstructionType::LOAD) {
        // call mesi function of load
        // according to cachestatus of hit or miss, call the busRequest operation
    } else if(instructionType == InstructionType::STORE) {
        // Handle store operation
    }
    
}

ProcessorState Processor::getState() {
    return state;
}

bool Processor::isDone() {
    return (state == ProcessorState::DONE);
}