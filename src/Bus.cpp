#include "headers/Bus.hpp"

Bus::Bus(int bandwidth){
    this->bandwidth = bandwidth;
}

void Bus::addProcessorToBus(Processor* processor) {
    processors.push_back(processor);
}

void Bus::processRequest(){
    //Updateinitial data
    //further call read or write function
}

void Bus::BusRead() {
    // Implement the logic for bus read operation
    // This function will be called when a processor wants to read data from the bus
    // It should handle the data transfer and update the state of the processors accordingly
}

void Bus::BusWrite() {
    // Implement the logic for bus write operation
    // This function will be called when a processor wants to write data to the bus
    // It should handle the data transfer and update the state of the processors accordingly
}