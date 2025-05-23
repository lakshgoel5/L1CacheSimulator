#include "headers/Bus.hpp"
#include <algorithm>

bool debug_bus = false;


Bus::Bus(int bandwidth){
    this->bandwidth = 4*bandwidth;
    this->currentRequest = nullptr;
}

int Bus::cycle(){
    //take element from ques
    //decrease it's countdown
    //if countdown = 0, then update cache line
    if(this->currentRequest == nullptr && !this->busQueue.empty()){
        if(debug_bus){
            cout << "Bus Queue is not empty, so taking request from queue" << endl;
            cout << "Bus queue contains request of processor ID and address " << endl;
            //print all processor IDs and correesponding addresses in the queue
            queue<Request> tempQueue = busQueue;
            while(!tempQueue.empty()){
                Request tempRequest = tempQueue.front();
                cout << "Processor ID: " << tempRequest.processorID << " Address: " << hex << tempRequest.address << dec << endl;
                tempQueue.pop();
            }
        }
        currentRequest = &busQueue.front();
        processRequest(currentRequest);
        currentRequest->counter = currentRequest->eviction + currentRequest->other_back + currentRequest->self_get;
        currentRequest->totalCounter = currentRequest->counter;
    }
    if(currentRequest != nullptr){
        bool allHalted = true;
        for(int i=0; i<processors.size(); i++){
            if(processors[i]->halted == false && processors[i]->isDone() == false){
                allHalted = false;
                break;
            }
        }

        //if all are not halted, go cycle by cycle
        if(!allHalted){
            //print queue
            currentRequest->counter--;

            if(currentRequest->counter == 0){
                busQueue.pop();
                this->processors[currentRequest->processorID]->halted = false;
                processors[currentRequest->processorID]->updatecacheState(currentRequest->address, currentRequest->toBeUpdatedState);
                processors[currentRequest->processorID]->updateStateToFree();  // what if the last instruction is only left -> then has to be set to done
                currentRequest = nullptr;
            }

            return 0;
        }



        //if halted, then jump
        else{
            int jump = currentRequest->counter;
            //update processor cycles
            for(int i=0; i<processors.size(); i++){
                if(processors[i]->isDone() == false){
                    if(!otherBack(i)){
                        if(busQueue.front().counter <= busQueue.front().totalCounter - busQueue.front().other_back){
                            processors[i]->numOfCycles += jump;
                        }
                        else{
                            processors[i]->IdleCycles = busQueue.front().other_back - (busQueue.front().totalCounter - busQueue.front().counter);
                            processors[i]->numOfCycles += busQueue.front().self_get + busQueue.front().eviction;
                        }
                    }
                    else{
                        processors[i]->IdleCycles += jump;
                    }
                }
            }
            busQueue.pop();
            currentRequest->counter = 0;
            this->processors[currentRequest->processorID]->halted = false;
            processors[currentRequest->processorID]->updatecacheState(currentRequest->address, currentRequest->toBeUpdatedState);
            processors[currentRequest->processorID]->updateStateToFree();
            currentRequest = nullptr;

            return jump;
        }
    }
    return 0;
}

void Bus::processRequest(Request* request) {
    if(request->type == TransactionType::BUSRD) {
        processRD(request);
    } else if(request->type == TransactionType::BUSRDX) {
        processRDX(request);
    }
}

void Bus::processRD(Request* request) {
    //first figure out if address is present in other caches or not, dpending on that, call cache update
    bool ispresent= false;
    int blocksize = processors[request->processorID]->getBlockSize();
    int is_present_id = -1;

    // see other processors
    for(int i=0; i<processors.size(); i++){

        if (i == request->processorID) {continue;}  // Skip the current processor

        MESIState state = processors[i]->getCacheState(request->address);


        if(state == MESIState::E || state == MESIState::S) {
            ispresent = true;
            if(is_present_id == -1){
                is_present_id = i;
            }
            processors[i]->updatecacheState(request->address, MESIState::S); //goes to shared state in case of MEM_READ signal, see assets
        }



        else if(state == MESIState::M){
            ispresent = true;
            if(is_present_id == -1){
                is_present_id = i;
            }
            processors[i]->updatecacheState(request->address, MESIState::S); //goes to shared state in case of MEM_READ signal, see assets

            //copy back
            processors[i]->dataTraffic += blocksize; // block size same for all
            this->totalBusTraffic += blocksize;
            request->other_back += 100; // write back to memory of other cache
            processors[i]->numWriteBack++;
        }
    }

    int add_line = processors[request->processorID]->addCacheLine(request->address, MESIState::I); //initially sending I state
    if(add_line > 0){
        request-> eviction += add_line;
        // M state block is evicted as LRU
        processors[request->processorID]->dataTraffic += blocksize;
        this->totalBusTraffic += blocksize;
    }

    if(ispresent == false){
        //read from memory
        request->self_get += 100;

        processors[request->processorID]->dataTraffic += blocksize;
        this->totalBusTraffic += blocksize;

        currentRequest->toBeUpdatedState = MESIState::E;

        if(debug_bus){cout << "Data not present in other caches, so setting self_get to " << request->counter<< endl;}
    }
    else{
        request->self_get += blocksize/2;

        processors[request->processorID]->dataTraffic += blocksize;
        this->totalBusTraffic += blocksize;
        processors[is_present_id]->dataTraffic += blocksize;

        currentRequest->toBeUpdatedState = MESIState::S;

        if(debug_bus){cout << "Data present in other caches, so setting counter to " << request->counter << endl;}
    }
}
// cache eviction of modified block - left
// handling simultaneous reads/writes to the same memory address - left

void Bus::processRDX(Request* request) {
    if(debug_bus){cout << endl;cout << "Inside ProcessRDX function" << endl;}

    int blocksize = processors[request->processorID]->getBlockSize();
    bool ispresent = false;
    int is_present_id = -1;

    //go to each processor
    for(int i=0; i<processors.size(); i++){

        if (i == request->processorID) {continue;}  // Skip the current processor

        MESIState state = processors[i]->getCacheState(request->address);

        if(debug_bus){cout << "Checking for data in Cache of Processor number " << i << endl;}

        if(state == MESIState::M) { //debug uodatecachestate or invaidate it to remove it? i.e make it invalid
            processors[i]->updatecacheState(request->address, MESIState::I); //goes to invalid state in case of RWITM or INVALIDATE signal
            ispresent = true;

            if(is_present_id == -1){
                is_present_id = i;
            }
            
            //copy back
            processors[i]->dataTraffic += blocksize;
            this->totalBusTraffic += blocksize;
            request->other_back+= 100; // write back to memory of other cache
            processors[i]->numWriteBack++;
        }


        else if(state == MESIState::S || state == MESIState::E) {
            processors[i]->updatecacheState(request->address, MESIState::I); //goes to invalid state in case of RWITM or INVALIDATE signal
            ispresent = true;
            if(is_present_id == -1){
                is_present_id = i;
            }
        }
    }


    int add_line = processors[request->processorID]->addCacheLine(request->address, MESIState::I); //initially sending I state
    if(add_line > 0){
        // M state block is evicted as LRU
        processors[request->processorID]->dataTraffic += blocksize;
        this->totalBusTraffic += blocksize;
        request->eviction += add_line;
    }


    currentRequest->toBeUpdatedState = MESIState::M;

    if(request->transaction == BusTransaction::INVALIDATE){
        //just send Invalidate and write on it
        processors[request->processorID]->numBusInvalidate++;
        request->self_get += 1;
    }
    else if(ispresent == false){
        //read from memory
        request->self_get += 100;
        processors[request->processorID]->dataTraffic += blocksize;
    }
    else{
        // int n = 1<<(b-2); // debug
        request->self_get += blocksize/2; //transfer

        processors[request->processorID]->numBusInvalidate++;
        
        processors[request->processorID]->dataTraffic += blocksize;
        processors[is_present_id]->dataTraffic += blocksize;
        this->totalBusTraffic += blocksize;
    }
}

void Bus::addToQueue(Request request) {
    busQueue.push(request);
}

bool Bus::isDone() {
    return busQueue.empty() && currentRequest == nullptr;
}

void Bus::haltProcessor(int processorID){
    this->processors[processorID]->halted = true;
}

void Bus::addProcessorToBus(Processor* processor) {
    processors.push_back(processor);
}

long long Bus::getBusTraffic() {
    totalBusTraffic = 0;
    for(int i=0; i<processors.size(); i++){
        totalBusTraffic += processors[i]->dataTraffic;
    }
    return totalBusTraffic;
}

long long Bus::maxExecutionTime() {
    long long maxTime = 0;
    for(int i=0; i<processors.size(); i++){
        if(processors[i]->numOfCycles > maxTime){
            maxTime = std::max(maxTime, static_cast<long long>(processors[i]->numOfCycles));
        }
    }
    return maxTime;
}
bool Bus::otherBack(int i){
    if(busQueue.empty()){
        return false;
    }
    if( busQueue.front().processorID == i){
        return false;
    }
    return true;
}