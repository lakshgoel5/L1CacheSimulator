#include "headers/Bus.hpp"

bool debug_bus = true;

Bus::Bus(int bandwidth){
    this->bandwidth = bandwidth;
    this->currentRequest = nullptr;
}

void Bus::addProcessorToBus(Processor* processor) {
    processors.push_back(processor);
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

void Bus::cycle(){
    //take element from ques
    //decrease it's countdown
    //if countdown = 0, then update cache line
    if(this->currentRequest == nullptr && !this->busQueue.empty()){
        currentRequest = &busQueue.front();
        busQueue.pop();
        processRequest(currentRequest);
    }
    if(debug_bus){
        cout << "Current Request Processor ID: " << currentRequest->processorID << endl;
        cout << "Current Request transaction: ";
        if(currentRequest->transaction == BusTransaction::MEMREAD) {
            cout << "MEMREAD" << endl;
        } else if(currentRequest->transaction == BusTransaction::RWITM) {
            cout << "RWITM" << endl;
        } else if(currentRequest->transaction == BusTransaction::INVALIDATE) {
            cout << "INVALIDATE" << endl;
        }
        cout << "Address " << hex << currentRequest->address << dec << endl;
        cout << "Current Request Counter: " << currentRequest->counter << endl;
    }
    if(currentRequest != nullptr){
        currentRequest->counter--;
        if(currentRequest->counter == 0){
            if(debug_bus){
                cout<< "DONE Current Request: " << currentRequest->transaction << " " << hex << currentRequest->address << dec << endl;
            }
            //update cache line
            processors[currentRequest->processorID]->updatecacheState(currentRequest->address, currentRequest->toBeUpdatedState);
            processors[currentRequest->processorID]->updateStateToFree();
            currentRequest = nullptr;
        }
    }
}

void Bus::processRequest(Request* request) {
    if(!request->isToMemOrCache){
        return; //if the request is not to memory or cache, return
    }
    if(request->type == TransactionType::BUSRD) {
        if(debug_bus){
            cout << "Processing Bus Read" << endl;
        }
        processRD(request);
    } else if(request->type == TransactionType::BUSRDX) {
        if(debug_bus){
            cout << "Processing Bus RDX" << endl;
        }
        processRDX(request);
    }
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

void Bus::processRD(Request* request) {
    //first figure out if address is present in other caches or not, dpending on that, call cache update
    bool ispresent= false;
    if(debug_bus){
        cout << endl;
        cout << "Inside ProcessRD function" << endl;
    }

    // see other processors
    for(int i=0; i<processors.size(); i++){
        MESIState state = processors[i]->getCacheState(request->address);
        if(debug_bus){
            cout << "Checking for data in Cache of Processor number " << i << endl;
        }
        if(state == MESIState::E || state == MESIState::S) {
            ispresent = true;
            processors[i]->updatecacheState(request->address, MESIState::S); //goes to shared state in case of MEM_READ signal, see assets
            if(debug_bus){
                cout << "Data present in other caches, so is_present is true and state is";
                if (state == MESIState::E) {
                    cout << "E" << endl;
                }
                else if (state == MESIState::S) {
                    cout << "S" << endl;
                }
                else if (state == MESIState::M)
                    cout << "M" << endl;
            }
        }
        else if(state == MESIState::M){
            ispresent = true;
            processors[i]->updatecacheState(request->address, MESIState::S); //goes to shared state in case of MEM_READ signal, see assets
            //copy back
            request->counter += 100; // write back to memory of other cache
            processors[i]->numWriteBack++;
            if(debug_bus){
                cout << "Data present in other caches, so is_present is true and state is";
                if (state == MESIState::E) {
                    cout << "E" << endl;
                }
                else if (state == MESIState::S) {
                    cout << "S" << endl;
                }
                else if (state == MESIState::M)
                    cout << "M" << endl;
                cout << "Due to M state, so copy back to memory. Request counter changed to " << request->counter << endl;
            }

        }
    }
    //add cache line
    //.......
    request->counter += processors[request->processorID]->addCacheLine(request->address, MESIState::I); //initially sending I state
    if(ispresent == false){
        // this goes to cycle processors[request.processorID]->updatecacheState(request.address, MESIState::E); //goes to exclusive //goes to exclusive
        //read from memory
        if(debug_bus){
            cout << "request counter before updating is " << request->counter << endl;
        }
        request->counter += 100;
        if(debug_bus){
            cout << "request counter after updating is " << request->counter << endl;
        }
        currentRequest->toBeUpdatedState = MESIState::E;
        if(debug_bus){
            cout << "Data not present in other caches, so setting counter to " << request->counter << endl;
        }
        // left
    }
    else{
        // stalling will occur - 2*N cycles
        //read from other cache
        // left
        int b = processors[request->processorID]->getBlockSize();
        int n = 1<<(b-2); // debug
        request->counter += 2*n;
        currentRequest->toBeUpdatedState = MESIState::S;
        if(debug_bus){
            cout << "Data present in other caches, so setting counter to " << request->counter << endl;
        }
    }
}
// cache eviction of modified block - left
// handling simultaneous reads/writes to the same memory address - left

void Bus::processRDX(Request* request) {
    bool ispresent = false;
    for(int i=0; i<processors.size() && i!=request->processorID && i!=request->processorID; i++){
        MESIState state = processors[i]->getCacheState(request->address);
        if(state == MESIState::M) { //debug uodatecachestate or invaidate it to remove it? i.e make it invalid
            processors[i]->updatecacheState(request->address, MESIState::I); //goes to invalid state in case of RWITM or INVALIDATE signal
            ispresent = true;
            //copy back
            request->counter += 100; // write back to memory of other cache
            processors[i]->numWriteBack++;
        }
        else if(state == MESIState::S || state == MESIState::E) {
            processors[i]->updatecacheState(request->address, MESIState::I); //goes to invalid state in case of RWITM or INVALIDATE signal
            ispresent = true;
        }
    }
    request->counter += processors[request->processorID]->addCacheLine(request->address, MESIState::I); //initially sending I state
    currentRequest->toBeUpdatedState = MESIState::M;
    if(ispresent == false){
        //read from memory
        request->counter += 100;
    }
    else{
        int b = processors[request->processorID]->getBlockSize();
        int n = 1<<(b-2); // debug
        request->counter += 2*n;
    }
}

void Bus::addToQueue(Request request) {
    busQueue.push(request);
}

bool Bus::isDone() {
    return busQueue.empty() && currentRequest == nullptr;
}