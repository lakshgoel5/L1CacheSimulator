#ifndef REQUEST_HPP
#define REQUEST_HPP
#include "utils.hpp"

class Request {
  public:
    bool done = false;
    BusTransaction transaction;
    unsigned int processorID;
    TransactionType type;
    unsigned int address;
    int counter = 0;
    MESIState toBeUpdatedState;
    int eviction = 0; //my cache full, so evict
    int other_back = 0;
    int self_get = 0;
    int totalCounter = 0;

    Request(BusTransaction transaction, unsigned int processorID, TransactionType type, unsigned int address): transaction(transaction) ,processorID(processorID), type(type), address(address){};
};

#endif