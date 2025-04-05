# TODO - Cache Coherence Simulator

- [ ] 1. Memory address is 32-bit. If any address is less than 32 bit, assume remaining MSB to be 0. E.g. the address 0x817b08 is actually 0x00817b08.  
      ⮕ [L1simulate]:

- [ ] 2. Each memory reference accesses 32-bit (4-bytes) of data. That is word size is 4- bytes.  
      ⮕ [Cache.hpp]:

- [X] 3. We are only interested in the data cache and will not model the instruction cache.  
      ⮕ [sort]:

- [X] 4. Each processor has its own L1 data cache. The L1 data caches are backed up by main memory — there is no L2 data cache.  
      ⮕ [Only one cache sort]:

- [ ] 5. L1 data cache uses write-back, write-allocate policy and LRU replacement policy.  
      ⮕ [in mesi.cpp]:

- [ ] 6. L1 data caches are kept coherent using MESI cache coherence protocol.  
      ⮕ [mesi.cpp]:

- [X] 7. Initially all the caches are empty.  
      ⮕ [valid=false]:

- [ ] 8. Ties are broken arbitrarily on the bus, when multiple cores attempt bus transactions simultaneously.  
      ⮕ [bus.cpp]:

- [ ] 9. L1 cache hit is 1 cycle. Fetching a block from memory to cache takes additional 100 cycles. Sending a word from one cache to another (e.g., BusUpdate) takes only 2 cycles. However, sending a cache block with N words (each word is 4 bytes) to another cache takes 2N cycles. Assume that evicting a dirty cache block to memory when it gets replaced is 100 cycles.  
      ⮕ [un-sorted]:

- [ ] 10. Assume that the caches are blocking. That is, if there is a cache miss, the cache cannot process further requests from the processor core and the core is completely halted. However, the snooping transactions from the bus still need to be processed in the cache.  
      ⮕ [un-sorted]:

- [ ] 11. In each cycle, each core can execute at most one memory reference instruction.  
      ⮕ [un-sorted]:

- [ ] 12. Add code in the simulator to compute maximum execution time for any core for an application.  
      ⮕ [un-sorted]:

- [ ] 13. Your program should generate the following output in each run. You can generate additional output as well.    
⮕ [un-sorted]:
  - [ ] Number of read/write instructions per core
  - [ ] Total execution cycles per core
  - [ ] Number of idle cycles per core
  - [ ] Data cache miss rate for each core
  - [ ] Number of cache evictions per core
  - [ ] Number of writebacks per core
  - [ ] Number of invalidations on the bus
  - [ ] Amount of data traffic on the bus  


- [ ] 14. Submit the repo (without the memory traces)  
      ⮕ [end-day]:
