#include "headers/Processor.hpp"
#include "headers/Cache.hpp"
#include "headers/Bus.hpp"
#include "headers/utils.hpp"
#include "headers/Memory.hpp"
#include <cmath>
#include <cstring>
//expand address
// Memory address is 32-bit. If any address is less than 32 bit, assume remaining MSB to be 0

using namespace std;

#define NUMCORES 4

bool debug = false; // Set to true for debugging
void printSimulationParameters(string tracePrefix, int setIndexBits, int Associativity, int blockBits) {
    cout << "Simulation Parameters:" << endl;
    cout << "Trace File: " << tracePrefix << endl;
    cout << "Set Index Bits: " << setIndexBits << endl;
    cout << "Associativity: " << Associativity << endl;
    cout << "Block Bits: " << blockBits << endl;
    cout << "Block Size: " << (1<<blockBits) << endl;
    cout << "Number of sets: " << (1<<setIndexBits) << endl;
    // numsets*associativity*block size
    int cacheSize = (1<<setIndexBits) * Associativity * (1<<blockBits);
    cout << "Cache size: " << cacheSize/1024 << endl;
    cout << "Mesi Protocol: Enabled"<<endl;
    cout << "Write Policy: Write-back, Write-allocate"<<endl;
    cout << "Replacement Policy: LRU" << endl;
    cout << "Bus: Central snooping bus" << endl;
}
int main(int argc, char* argv[]){
    unsigned long long stop_at_cycle = 1000000000000000; //default value //debug
    if(debug){
        cin >> stop_at_cycle;
    }
    // Parse command line arguments
    if(argc > 1 && strcmp(argv[1], "-h") == 0){
        cout << "Usage: " << argv[0] << " -t <tracefile> -s <number of set index bits> -E <number of lines per set> -b <block size in bytes> -o <output file>" << endl;
        return 0;
    }
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
    //trace file should be like app1
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

    if(debug){
        cout << "Trace File: " << traceFile << endl;
        cout << "Set Index Bits: " << setIndexBits << endl;
        cout << "Number of Lines: " << numLines << endl;
        cout << "Block Size: " << blockSize << endl;
        cout << "Output File: " << outputFile << endl;
    }

    size_t numSets = pow(2, setIndexBits);
    vector<Processor*> processorsInWork; //Number of processors can be variable
    Bus bus(blockSize); // bandwidth is assumed to be equal to block size for simplicity
    for(int i=0; i<NUMCORES; i++){
        Processor* processor = new Processor(i, numSets, numLines, blockSize, "../traces/" + traceFile + "_proc" + to_string(i) + ".trace", &bus);
        bus.addProcessorToBus(processor);
        processorsInWork.push_back(processor);
    }

    unsigned int clock = 0;
    while(true){
        bool AllDone = true;
        if(debug) cout << "---------------------------------Clock Cycle " << clock << "----------------------------------" << endl;
        for(int i=0; i<NUMCORES; i++){
            if(processorsInWork[i]->isDone() == false){
                AllDone = false;
                if(debug){
                    cout << "**********Current Processor " << i << "***********" << endl;
                }
                processorsInWork[i]->cycle();
            }
        }
        if(debug){
            cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^Starting bus cycle^^^^^^^^^^^^^^^^^^^^^^^^" << endl;
        }
        bus.cycle();
        if((AllDone && bus.isDone()) || clock > stop_at_cycle){
            break;
        }
        else{
            clock++;
        }
    }
    cout<<endl;
    printSimulationParameters(traceFile, setIndexBits, numLines, blockSize);
    cout<<endl;
    for(int i=0; i<NUMCORES; i++){
        processorsInWork[i]->PrintStats();
        cout<<endl;
    }
    return 0;
}