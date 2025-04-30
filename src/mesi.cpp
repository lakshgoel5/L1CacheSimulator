#include "headers/mesi.hpp"

bool debug_mesi = false; // Set to true for debugging

/// @brief returns whether cache hit or miss
/// @param processorID 
/// @param address 
/// @param bus 
/// @param cache 
/// @return HIT ot MISS
ProcessMESIResult MESIProtocol::read(int processorID, unsigned int address, Bus& bus, Cache& cache) {
    if(debug_mesi){cout << "Starting read execution in MESI protocol" << endl; }

    MESIState mesistate = cache.getState(address);//if data is not present in cache, return invalid state

    if(debug_mesi){
        cout<<"address is: "<<hex<<address<<dec<<endl;
        cout << "State of Cache line is : "; 
        if(mesistate == MESIState::S) { cout << "S" << endl; }
        else if(mesistate == MESIState::E) { cout << "E" << endl; }
        else if(mesistate == MESIState::M) { cout << "M" << endl; }
        else if(mesistate == MESIState::I) { cout << "I" << endl; }
    
    }

    // locally initiated changes
    if(mesistate == MESIState::S || mesistate == MESIState::E || mesistate == MESIState::M) {
        if(debug_mesi){cout << "Read hit" << endl; }
        cache.updateCacheState(address, mesistate); //debug try removing this readblock
        return CACHE_HIT;
    }

    //read miss
    else if(mesistate == MESIState::I) {
        if(debug_mesi){cout << "Read miss" << endl; }

        Request request(BusTransaction::MEMREAD, processorID, TransactionType::BUSRD, address);
        bus.busTransactions++;
        bus.addToQueue(request);
        bus.haltProcessor(processorID);
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
}

ProcessMESIResult MESIProtocol::write(int processorID, unsigned int address, Bus& bus, Cache& cache) {
    if(debug_mesi){cout << "Starting write execution in MESI protocol" << endl; }

    MESIState mesistate = cache.getState(address);//if data is not present in cache, return invalid state

    if(debug_mesi){
        cout<<"address is: "<<hex<<address<<dec<<endl;
        cout << "State of Cache line is : "; 
        if(mesistate == MESIState::S) { cout << "S" << endl; }
        else if(mesistate == MESIState::E) { cout << "E" << endl; }
        else if(mesistate == MESIState::M) { cout << "M" << endl; }
        else if(mesistate == MESIState::I) { cout << "I" << endl; }
    
    }


    if(mesistate == MESIState::S){
        if(debug_mesi){cout << "Write hit in shared state" << endl; }

        Request request(BusTransaction::INVALIDATE, processorID, TransactionType::BUSRDX, address);
        bus.busTransactions++;
        bus.addToQueue(request);
        bus.haltProcessor(processorID);
        //updatecache inside processRDX
        return CACHE_HIT;
    }

    //just modify data
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
        bus.busTransactions++;
        bus.addToQueue(request);
        bus.haltProcessor(processorID);
        //updatecache inside processRDX
        return CACHE_MISS;
    }
    else{
        throw runtime_error("invalid state");
    }
    // return result;
}