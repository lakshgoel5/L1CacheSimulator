#include "headers/mesi.hpp"

ProcessMESIResult MESIProtocol::read(unsigned int address, Bus& bus, Cache& cache) {
    MESIState mesistate = cache.getState(address);
    ProcessMESIResult result;
    // 
    // locally initiated changes

    //In Shared state -> READ_HIT
    //call updatecache with state = S (update chache function will set state to wht we send, and send it to lru)

    //In Exclusive state -> READ_HIT
    // send E to updatecache

    //in Modified state -> READ_HIT
    // send M to updatecache

    //In invalid state -> READ_MISS
    //bus request to MEMREAD
    //send either S or E to updatecache
    
    return result;
}

ProcessMESIResult MESIProtocol::write(unsigned int address, Bus& bus, Cache& cache) {
    ProcessMESIResult result;
    // in shared state -> WRITE_HIT
    // send INVALIDATE request to bus
    //cahnge state to M

    // in Modified state -> WRITE_HIT
    // change state to M

    // in exclusive state -> WRITE_HIT
    // change state to M

    //in invalid state -> WRITE_MISS
    // send bus request to RWITM
    // change state to M
    return result;
}