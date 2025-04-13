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

void Bus::incomingRequest(BusTransaction transaction, unsigned int address, int processorID) {
    //go to each processor other than the processor passed in function
    //go to their caches using processor->cache (point 10, snooping transactions still continue)
    //check if the address is present in their cache(if cache not present, you get I -> do nothing as written below)
    //if present(either M or E or S), check the state of that cache line

    //if state is M, and transaction is RWITM, then set it to Invalid, along with copy back (Write_back, as well as share block to other caches so 2*n (less than 100), then other cache modifies it)
    //(1)//if state is M, and transaction is MEMREAD, then set it to S, along with copy back (In this, write back, as well as share block to other caches so 2*n)(Why? coz 2*n less then 100 for original processor to read from mem)

    //if state is S, then set it to Invalid(transaction should be invalidate) -> call write function
    //if state S, and transaction is MEM_READ, then keep it to S -> call read function

    //if state is E, and transaction is RWITM, then set it to Invalid
    //(2)//if state is E, and transaction is MEMREAD, then set it to S

    //if state is I, then do nothing

    //for copy back, we have to set processor state to WRITE_MEMORY(Only if it is in free state and not doing an old read or write)(Now the question is have that prcessor executed it's present instruction or not)
    //and then call bus write function in Processor.cpp only


    //in case of MEM_READ or RWITM, set state to READ_MEMORY for current processor(already done by executeFree function in processor)

}