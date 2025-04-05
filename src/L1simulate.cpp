

//expand address
// Memory address is 32-bit. If any address is less than 32 bit, assume remaining MSB to be 0



//main
// $./L1simulate -h
// -t <tracefile>: name of parallel application (e.g. app1) whose 4 traces are to be used
// in simulation
// -s <s>: number of set index bits (number of sets in the cache = S = 2
// s)
// 1
// -E <E>: associativity (number of cache lines per set)
// -b <b>: number of block bits (block size = B = 2
// b)
// -o: <outfilename> logs output in file for plotting etc.
// -h: prints this help