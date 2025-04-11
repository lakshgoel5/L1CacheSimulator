#include "headers/Processor.hpp"

void Processor::cycle() {
    numOfCycles++;
    //update values
    // get the instruction from the trace file
    //call execute function or nay other if needed
}

void Processor::execute() {
    if(state == ProcessorState::FREE) {
        // Execute the instruction
        //update instruction type
        // call free function
        // Update the state based on the instruction
        // For example, if it's a load instruction, update the cache
        // If it's a store instruction, update the cache and bus
    } else if(state == ProcessorState::READ_MEMORY) {
        // Handle read memory operation
    } else if(state == ProcessorState::WRITE_MEMORY) {
        // Handle write memory operation
    } else if(state == ProcessorState::DONE) {
        // Finalize the operation
        
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