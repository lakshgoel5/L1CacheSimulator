#include "headers/CacheLine.hpp"
#include <stdexcept>
#include <cstdint>
#include <iostream>

using namespace std;

CacheLines::CacheLines(size_t blockSize){
    data.resize(blockSize);
    state = I; //Initial state is invalid
    tag = 0; //Initial tag is 0
    valid = false; //Initially not valid
}

void CacheLines::setState(MESIState state){
    this->state = state;
}

MESIState CacheLines::getState(){
    return state;
}

void CacheLines::setTag(unsigned int tag){
    this->tag = tag;
}
unsigned int CacheLines::getTag(){
    return tag;
}

bool CacheLines::isValid(){
    return valid;
}
void CacheLines::setValid(bool valid){
    this->valid = valid;
}

void CacheLines::writebyte(size_t offset, int32_t value){
    if(offset >= this->data.size()){
        throw out_of_range("index out of range: " + to_string(offset));
    }
    this->data[offset] = value;
}
int8_t CacheLines::readbyte(size_t offset){
    if(offset >= this->data.size()){
        throw out_of_range("index out of range: " + to_string(offset));
    }
    return data[offset];
}

int CacheLines::readword(size_t offset){
    if(offset >= this->data.size()){
        throw out_of_range("index out of range: " + to_string(offset));
    }
    int value = 0;
    for(size_t i = 0; i < 4; ++i){
        value |= (data[offset + i] << (i * 8));
    }
    return value;
}

void CacheLines::writeblock(vector<int8_t> data){
    if(data.size() != this->data.size()){
        throw std::runtime_error("Data size does not match cache line size");
    }
    this->data = data;
}

vector<int8_t> CacheLines::readblock(){
    return data;
}

void CacheLines::printCacheLine(){ //debug
    cout << "Cache Line State: " << state << endl;
    cout << "Tag: " << tag << endl;
    cout << "Valid: " << valid << endl;
    cout << "Data: ";
    for(auto byte : data){
        cout << (int)byte << " ";
    }
    cout << endl;
}