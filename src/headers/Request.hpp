#ifndef REQUEST_HPP
#define REQUEST_HPP
#include "utils.hpp"

class Request {
  public:
    bool done = false;
    BusTransaction transaction;
    unsigned int processorID;
    bool isToMemOrCache = true; // debug
    TransactionType type;
    unsigned int address;
    int countdown = 0;

    Request(BusTransaction transaction, unsigned int processorID, TransactionType type, unsigned int address): transaction(transaction) ,processorID(processorID), type(type), address(address){};
};

#endif