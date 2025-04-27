#include "headers/mesi.hpp"

bool debug_mesi = true; // Set to true for debugging

// returns whether cache hit or miss
ProcessMESIResult MESIProtocol::read(int processorID, unsigned int address, Bus& bus, Cache& cache) {
    if(debug_mesi){cout << "Starting read execution in MESI protocol" << endl; }
    MESIState mesistate = cache.getState(address);//if data is not present in cache, return invalid state
    if(debug_mesi){
        cout << "State of Cache line is : "; 
        if(mesistate == MESIState::S) { cout << "S" << endl; }
        else if(mesistate == MESIState::E) { cout << "E" << endl; }
        else if(mesistate == MESIState::M) { cout << "M" << endl; }
        else if(mesistate == MESIState::I) { cout << "I" << endl; }
    
    }
    // locally initiated changes

    //In Shared state -> READ_HIT
    //call updatecache with state = S (update chache function will set state to what we send, and send it to lru)

    //In Exclusive state -> READ_HIT
    // send E to updatecache

    //in Modified state -> READ_HIT
    // send M to updatecache
    // case of read hit
    if(mesistate == MESIState::S || mesistate == MESIState::E || mesistate == MESIState::M) {
        if(debug_mesi){cout << "Read hit" << endl; }
        cache.updateCacheState(address, mesistate); //debug try removing this readblock
        return CACHE_HIT;
    }

    //In invalid state -> READ_MISS
    //bus request to MEMREAD -> Mesi just sends reuqest of RD(shared read)
    //send either S or E to updatecache
    //send E if data is not present in other caches
    //send S if data is present in other caches //doubt put on piazza
    // case of read miss
    else if(mesistate == MESIState::I) {
        if(debug_mesi){cout << "Read miss" << endl; }
        Request request(BusTransaction::MEMREAD, processorID, TransactionType::BUSRD, address);
        // if bus is busy --> queue me daal // debug
        // else ---> just process the request in the bus
        if(debug_mesi){
            cout << "Adding request to bus queue" << endl; 
            cout << "Request type: " << request.type << endl; 
            cout << "Request address: " << request.address << endl;
            cout << "Request processor ID: " << request.processorID << endl; 
            cout << "Bus transaction type: " << request.transaction << endl; 
        }
        bus.addToQueue(request);
        //updatecachestate done inside this request
        return CACHE_MISS;
        /*
        When a processor experiences a read miss, it sends a signal like BusRd (for shared read) or BusRdX (for exclusive access) to the bus. This is what the notes refer to when they say the MEM_READ signal should go to the bus. The processor doesn’t explicitly send a message saying “get data from another cache”; it simply broadcasts a read request.

        Then, it is the responsibility of other caches (that are snooping on the bus) to respond if they have the block in a valid state (like Shared, Exclusive, or Modified). If one of them has it, it can supply the data directly to the requesting cache. If not, the block is fetched from main memory.
        */
    }
    else{
        throw runtime_error("invalid state");
    }
    // return result;
}

ProcessMESIResult MESIProtocol::write(int processorID, unsigned int address, Bus& bus, Cache& cache) {
    if(debug_mesi){cout << "Starting write execution in MESI protocol" << endl; }
    MESIState mesistate = cache.getState(address);//if data is not present in cache, return invalid state
    if(debug_mesi){cout << "State of Cache line is : " << mesistate << endl; }
    // ProcessMESIResult result;
    // in shared state -> WRITE_HIT
    // send INVALIDATE request to bus -> mesi just sends request of RDX(exclusive write)
    //(go to other caches, check if that address present, if present then check the state of that cache line)
    //cahnge state to M
    if(mesistate == MESIState::S){
        if(debug_mesi){cout << "Write hit in shared state" << endl; }
        Request request(BusTransaction::INVALIDATE, processorID, TransactionType::BUSRDX, address);
        if(debug_mesi){
            cout << "Adding request to bus queue" << endl; 
            cout << "Request type: " << request.type << endl; 
            cout << "Request address: " << request.address << endl;
            cout << "Request processor ID: " << request.processorID << endl; 
            cout << "Bus transaction type: " << request.transaction << endl; 
        }
        bus.addToQueue(request);
        //updatecache inside processRDX
        return CACHE_HIT;
    }

    // in Modified state -> WRITE_HIT
    // change state to M
    else if(mesistate == MESIState::M){
        if(debug_mesi){cout << "Write hit in modified state" << endl; }
        cache.updateCacheState(address, mesistate); //debug try removing this readblock
        return CACHE_HIT;
    }

    // in exclusive state -> WRITE_HIT
    // change state to M
    else if(mesistate == MESIState::E){
        if(debug_mesi){cout << "Write hit in exclusive state" << endl; }
        cache.updateCacheState(address, MESIState::M); //debug try removing this readblock
        return CACHE_HIT;
    }

    //in invalid state -> WRITE_MISS
    // send bus request to RWITM -> Mesi just sends reuqest of RDX (exclusive write)
    // change state to M
    else if(mesistate == MESIState::I){
        if(debug_mesi){cout << "Write miss" << endl; }
        Request request(BusTransaction::RWITM, processorID, TransactionType::BUSRDX, address);
        if(debug_mesi){
            cout << "Adding request to bus queue" << endl; 
            cout << "Request type: " << request.type << endl; 
            cout << "Request address: " << request.address << endl;
            cout << "Request processor ID: " << request.processorID << endl; 
            cout << "Bus transaction type: " << request.transaction << endl; 
        }
        bus.addToQueue(request);
        //updatecache inside processRDX
        return CACHE_MISS;
    }
    else{
        throw runtime_error("invalid state");
    }
    // return result;
}