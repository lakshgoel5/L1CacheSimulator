#ifndef MESI_HPP
#define MESI_HPP
#include "utils.hpp"

//What MESI do on load and store
class MESIProtocol {
    public:
        ProcessResult read();
        ProcessResult write();
};

#endif // MESI_HPP