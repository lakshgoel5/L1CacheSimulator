#include "headers/Processor.hpp"

Processor::Processor(int processorID, size_t numSets, size_t numLines, size_t blockSize, string traceFile)
    : processorID(processorID), numOfCycles(0), state(ProcessorState::FREE), cache(numSets, numLines, blockSize) {
        // read trace file
        //save data in a vector, make it in .hpp file
}

void Processor::cycle() {
    numOfCycles++;
    //update values for stats
    //check state of processor
    //call execute function or any other if needed
}

void Processor::execute() {
    if(state == ProcessorState::FREE) {
        // Execute the instruction
        // get the instruction from the vector, one at a time
        // update instruction type
        // call execute_free function

        //increment index of vector of instrcution to next if it's a hit
        //call bus with appropriate singal
        
        //stay at same index if it's a miss
        //change state to read_memory or write_memory depending on R or W
        //call bus with approprite signal
        //further processing required

        //If I reach to end of vector of instructions, set state to done
    } else if(state == ProcessorState::READ_MEMORY) {
        // stay here until num of idle cycles = 100

        // if num of idle cycles = 100 then set state to free and read from memory update the cache value

        //If I reach to end of vector of instructions, set state to done
    } else if(state == ProcessorState::WRITE_MEMORY) {
        // stay here until num of idle cycles = 100

        // if num of idle cycles = 100 then set state to free and write in memory update the cache value

        //If I reach to end of vector of instructions, set state to done
    } else if(state == ProcessorState::DONE) {
        // Finalize the operation
        //break out of the function and return
        
    }
}

void Processor::execute_free(InstructionType instructionType) {
    if(instructionType == InstructionType::LOAD) {
        // call mesi function of load
        // get result which is hit or miss(Other work done by MESI only)

        //if miss, I have sent bus request to read from memory, and update cache transition to required status
        //so no need to do anything

        // if hit then set state to FREE
        // if not hit, then set state to READ_MEMORY, so that I can stay there for 100 cycles
    } else if(instructionType == InstructionType::STORE) {
        // call mesi function of store
        // get result which is hit or miss(Other work done by MESI only)

        //if miss, I have sent bus request to read from memory, and update cache transition to required status
        //so no need to do anything

        // if hit then set state to FREE
        // if not hit, then set state to WRITE_MEMORY, so that I can stay there for 100 cycles
    }
    
}

ProcessorState Processor::getState() {
    return state;
}

bool Processor::isDone() {
    return (state == ProcessorState::DONE);
}