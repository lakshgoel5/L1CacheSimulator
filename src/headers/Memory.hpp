#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <array>
#include <random>
#include <iostream>

using namespace std;

class Memory{
    private:
    static constexpr int memory_size = 1024*4; // 16KB memory
    std::array<int, memory_size> dataMemory;

    public:
    Memory();
    int readMemory(unsigned int address);
    void writeMemory(unsigned int address, int value);
};

#endif