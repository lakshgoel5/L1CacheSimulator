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
    cout << "Block Size (Bytes): " << (1<<blockBits) << endl;
    cout << "Number of sets: " << (1<<setIndexBits) << endl;
    // numsets*associativity*block size
    float cacheSize = (1<<setIndexBits) * Associativity * (1<<blockBits);
    cout << "Cache size (KB per core): " << cacheSize/1024 << endl;
    cout << "Mesi Protocol: Enabled"<<endl;
    cout << "Write Policy: Write-back, Write-allocate"<<endl;
    cout << "Replacement Policy: LRU" << endl;
    cout << "Bus: Central snooping bus" << endl;
}
int main(int argc, char* argv[]){
    // Parse command line arguments
    if(argc == 2 && strcmp(argv[1], "-h") == 0){
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
    int blockBits = 0;
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
            blockBits = stoi(argv[++i]);
            blockSize = (1<<blockBits);
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
        cout << "Block Bits: " << blockBits << endl;
        cout << "Block Size: " << blockSize << endl;
        cout << "Output File: " << outputFile << endl;
    }

    size_t numSets = pow(2, setIndexBits);
    vector<Processor*> processorsInWork; //Number of processors can be variable
    Bus bus(blockSize); // bandwidth is assumed to be equal to 4*block size for simplicity
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
            if(debug){
                cout << "******Cache of Processor " << i << "******" << endl;
                cout << "Cache State: ";
                processorsInWork[i]->printCache() ;
                cout << endl;
            }
            //if processor is not done, then call cycle function
            if(processorsInWork[i]->isDone() == false){
                AllDone = false;
                //If processor is not halted, then call cycle function
                if(!processorsInWork[i]->halted){
                    if(debug){cout << "**********Current Processor " << i << "***********" << endl;}

                    processorsInWork[i]->cycle();
                }
                //Processor halted, so increment its cycles and idlecycles
                else{
                    if(debug){cout << "Processor " << i << " is halted" << endl;}
                    if(!bus.otherBack(i)){
                        processorsInWork[i]->numOfCycles++;
                    }
                    else{
                        processorsInWork[i]->IdleCycles++;
                    }
                }
            }
        }
        if(debug){cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^Starting bus cycle^^^^^^^^^^^^^^^^^^^^^^^^" << endl;}
        //jump is the number of cycles left for current request to be executed
        int jump = bus.cycle();
        if((AllDone && bus.isDone())){
            //should stip only if Processor are done, and bus is empty
            break;
        }
        else if(jump == 0){
            //there is no jump, i.e. one of the processors is not halted
            clock++;
        }
        else{
            //change clock cycles by jump
            clock += jump;
        }
    }
    //Print stats
    cout<<endl;
    printSimulationParameters(traceFile, setIndexBits, numLines, blockBits);
    cout<<endl;
    
    for(int i=0; i<NUMCORES; i++){
        processorsInWork[i]->PrintStats();
        cout<<endl;
    }
    cout << "Overall Bus Summary:" << endl;
    cout << "Total Bus Transactions: " << bus.busTransactions << endl;
    cout << "Total Bus Traffic (Bytes): " << bus.totalBusTraffic << endl;
    return 0;
}