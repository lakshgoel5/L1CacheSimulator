<br />
<div align="center">

  <h1 align="center">L1 Cache Simulator for Quad-Core Processors</h1>
  <p align="center">
    This project implements a detailed simulator for L1 caches in a quad-core processor system using the MESI (Modified-Exclusive-Shared-Invalid) cache coherence protocol.
  </p>

</div>

### Authors
- Laksh Goel (2023CS10848)
- Yuvika Chaudhary (2023CS10353)

# Overview
The simulator models a multi-core system with the following features:
- MESI cache coherence protocol implementation
- Write-back, write-allocate policy
- LRU (Least Recently Used) replacement policy
- Central snooping bus for inter-cache communication
- Support for various cache configurations (sets, associativity, block size)


# Project Structure

```
L1-Cache-Simulator/
├── headers/                  # Header files
│   ├── Bus.hpp               # Bus interface definitions
│   ├── Cache.hpp             # Cache structure definitions
│   ├── CacheLine.hpp         # Cache line structure
│   ├── CacheSet.hpp          # Cache set implementation
│   ├── Memory.hpp            # Memory system interface
│   ├── mesi.hpp              # MESI protocol implementation
│   ├── Processor.hpp         # Processor implementation
│   └── utils.hpp             # Utility functions
├── src/                      # Source files
│   ├── Bus.cpp               # Bus implementation
│   ├── Cache.cpp             # Cache implementation
│   ├── CacheLine.cpp         # Cache line implementation
│   ├── CacheSet.cpp          # Cache set implementation
│   ├── L1simulate.cpp        # Main simulation driver
│   ├── Memory.cpp            # Memory implementation
│   ├── mesi.cpp              # MESI protocol implementation
│   ├── Processor.cpp         # Processor implementation
│   └── utils.cpp             # Utility functions
├── traces/                   # Trace files for simulation
│   ├── app1_proc0.trace      # Instruction trace for core 0
│   ├── app1_proc1.trace      # Instruction trace for core 1
│   └── ...                   # Other trace files
├── Makefile                  # Build system
└── README.md                 # This file
```
# Getting Started

## Prerequisites
- C++ compiler with C++11 support (g++ recommended)
- Linux operating system (tested on Ubuntu/Debian)
- make utility

## Building Project
### 1. Clone the repository and navigate to the project directory:
```bash
git clone https://github.com/lakshgoel5/L1-Cache-Simulator-COL216
cd L1-Cache-Simulator-COL216
```
### 2. Compiling the Project
To compile the simulator, navigate to the src directory and use make:
```bash
cd src
make
```

This will compile the simulator and create the executable `L1simulate`.

### 3. Running the Simulator
The simulator can be run with various configurations using command-line arguments:
```bash
./L1simulate -t <tracefile> -s <sets> -E <associativity> -b <blocksize> -o <output_file>
```

### Command-line Parameters
- `-t`: Trace file prefix (e.g., app1, app2)
- `-s`: Number of set index bits (S = 2^s is the number of sets)
- `-E`: Associativity (number of lines per set)
- `-b`: Number of block bits (B = 2^b is the block size)
- `-o`: Output file for simulation results
- `-h`: Display help message


### Trace file format
Each trace file contains a sequence of memory operations, one per line:
- ``R 0x12345678``: Read operation at memory address 0x12345678
- ``W 0x87654321``: Write operation at memory address 0x87654321

### Simulation Statistics
The simulator generates a detailed report of cache hits, misses, and other statistics for each core. The output file will contain:
- Total Instructions executed
- Total read operations
- Total write operations
- Total execution cycles
- Idle cycles
- Cache miss rate
- Cache Evictions
- Writebacks
- Bus Invalidations
- Data Traffic (in Bytes)

## MESI Protocol Implementation

## Assumptions