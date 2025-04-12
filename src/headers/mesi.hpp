#ifndef MESI_HPP
#define MESI_HPP
#include "utils.hpp"
#include "Bus.hpp"
#include "Cache.hpp"

//What MESI do on load and store
class MESIProtocol {
    public:
        ProcessMESIResult read(unsigned int address, Bus& bus, Cache& cache);
        ProcessMESIResult write(unsigned int address, Bus& bus, Cache& cache);
};

#endif // MESI_HPP