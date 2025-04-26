#pragma once
// #include "utils.hpp"
#include "Bus.hpp"

//What MESI do on load and store
class MESIProtocol {
    public:
        ProcessMESIResult read(int processorID, unsigned int address, Bus& bus, Cache& cache);
        ProcessMESIResult write(int processorID, unsigned int address, Bus& bus, Cache& cache);
};