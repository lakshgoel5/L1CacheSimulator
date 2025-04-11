#include "headers/Processor.hpp"
#include "headers/Cache.hpp"
#include "headers/Bus.hpp"
#include "headers/utils.hpp"
#include "headers/Memory.hpp"
#include <cmath>
//expand address
// Memory address is 32-bit. If any address is less than 32 bit, assume remaining MSB to be 0

using namespace std;

int main(int argc, char* argv[]){
    // Parse command line arguments
    if (argc != 11) { //debug
        cerr << "Try: " << argv[0] << " -h" << endl;
        return 1;
    }
    //debug, check int vs size_t in Cache.hpp
    //debug: what is string tracefile?
    string traceFile;
    int setIndexBits = 0;
    int numLines = 0;
    int blockSize = 0;
    string outputFile;

    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == "-t") {
            traceFile = argv[++i];
        } else if (string(argv[i]) == "-s") {
            setIndexBits = stoi(argv[++i]);
        } else if (string(argv[i]) == "-E") {
            numLines = stoi(argv[++i]);
        } else if (string(argv[i]) == "-b") {
            blockSize = stoi(argv[++i]);
        } else if (string(argv[i]) == "-o") {
            outputFile = argv[++i];
        } else if (string(argv[i]) == "-h") {
            cout << "Usage: " << argv[0] << " -t <tracefile> -s <number of set index bits> -E <number of lines per set> -b <block size in bytes> -o <output file>" << endl;
            return 0;
        }
    }

    if(traceFile.empty() || setIndexBits <= 0 || numLines <= 0 || blockSize <= 0 || outputFile.empty()) {
        cerr << "Invalid arguments. Use -h for help." << endl;
        return 1;
    }

    size_t numSets = pow(2, setIndexBits);
    Cache cache(numSets, numLines, blockSize);
    Bus bus(blockSize); // bandwidth is assumed to be equal to block size for simplicity
    Processor processor(1 << setIndexBits, numLines, blockSize);

    return 0;
}