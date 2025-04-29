#pragma once
#include "Processor.hpp"
#include <vector>
#include <iostream>
#include <string>
#include "Request.hpp"
#include <queue>
//For implementing wires
//Ties are broken arbitrarily on the bus, when multiple cores attempt bus transactions simultaneously.

//to show
// Number of invalidations on the bus.
// Amount of data traffic (in bytes) on the bus.

using namespace std;

class Processor; // Forward declaration
class Request; // Forward declaration

class Bus {
    private:
        vector<Processor*> processors;
        queue<Request> busQueue;
        int bandwidth;
        Request* currentRequest;

    public:
        int busTransactions = 0;
        int totalBusTraffic = 0;
        void addToQueue(Request request);
        Bus(int bandwidth);
        void addProcessorToBus(Processor* processor);
        void BusRead();
        void BusWrite();
        void processRequest(Request* request);
        void processRD(Request* request);
        void processRDX(Request* request);
        int cycle();
        bool isDone();
        void haltProcessor(int processorID);

};