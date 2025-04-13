#ifndef BUS_HPP
#define BUS_HPP
#include "Processor.hpp"
#include <vector>
#include <iostream>
#include <string>
//For implementing wires
//Ties are broken arbitrarily on the bus, when multiple cores attempt bus transactions simultaneously.

//to show
// Number of invalidations on the bus.
// Amount of data traffic (in bytes) on the bus.

using namespace std;

class Bus {
    private:
        vector<Processor*> processors;
        int bandwidth;

    public:
        Bus(int bandwidth);
        void addProcessorToBus(Processor* processor);
        void BusRead();
        void BusWrite();
        void processRequest(BusTransaction transaction, unsigned int address, int processorID );

};
#endif // BUS_HPP