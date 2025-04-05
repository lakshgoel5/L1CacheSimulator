#include "headers/Memory.hpp"
#include <stdexcept>

Memory::Memory(){
    // Random number generator
    std::random_device rd;   // Non-deterministic random number generator
    std::mt19937 gen(rd());  // Mersenne Twister engine
    std::uniform_int_distribution<uint32_t> dist(0, 1023); // Range for uint32_t

    // Fill memory with random numbers
    for(auto& val : dataMemory) {
        val = dist(gen);
    }
    // std::fill(dataMemory.begin(), dataMemory.end(), 0);
}

int Memory::readMemory(unsigned int address){
    if(address > memory_size){
        throw out_of_range("index out of range: " + to_string(address)); //Raises an exception of type out_of_range
    }
    return dataMemory[address];
}

void Memory::writeMemory(unsigned int address, int value){
    if(address > memory_size){
        throw out_of_range("index out of range: " + to_string(address));
    }
    dataMemory[address] = value;
    return;
}