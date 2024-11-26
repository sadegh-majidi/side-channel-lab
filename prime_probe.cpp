#include "prime_probe.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <x86intrin.h>
#include <sys/mman.h>
#include <cassert>
#define PAGE_SIZE 4096
void prime_probe::setup(prime_probe::prime_probe_buffer& pb, prime_probe::results& res,prime_probe::cache_sets& set,
                        uint32_t number_of_sets, uint32_t number_of_ways, uint32_t block_size){
    pb.number_of_sets = number_of_sets;
    pb.number_of_ways = number_of_ways;

    pb.eviction_sets = (void**)malloc(/*tomato*/);
    res.times = (uint64_t*)aligned_alloc(/*tomato*/, /*tomato*/);
    // We want to make sure we dont use the same page for multiple ways
	void* pointers = aligned_alloc(/*tomato*/, /*tomato*/*PAGE_SIZE);
        void** pointer_array = (void**)malloc(/*tomato*/);
	// A cache set so that we can measure the time it takes to access the cache set
	void* underlying_for_eviction_sets = aligned_alloc(/*tomato*/, /*tomato*/*PAGE_SIZE);
	set.eviction_sets = (void**)malloc(/*tomato*/);
    set.number_of_sets = number_of_sets;
    set.number_of_ways = number_of_ways;

	for(int cache_set=0; cache_set < number_of_sets; cache_set++){
		for (int i = 0; i < number_of_ways; i++) pointer_array[i] = (void *)((uintptr_t)pointers + /*tomato*/ + i * PAGE_SIZE);
                // We want to make a linked list for the eviction sets.
                // tomato: if you want you can also introduce a shuffle for the pointer array
		for(int j = 0; j < number_of_ways; j++){
		    *((void**)pointer_array[j]) = pointer_array[/*tomato*/];
		}
		pb.eviction_sets[cache_set] = pointer_array[0];
		for (int i = 0; i < number_of_ways; i++) pointer_array[i] = (void *)((uintptr_t)underlying_for_eviction_sets + /*tomato*/ + i * PAGE_SIZE);
		for(int j = 0; j < number_of_ways; j++){
		    *((void**)pointer_array[j]) = pointer_array[/*tomato*/];
		}
		set.eviction_sets[cache_set] = pointer_array[0];
                assert(set.eviction_sets[cache_set] != pb.eviction_sets[cache_set]);
	}

	free(pointer_array);


	uint64_t start = __rdtsc();
	while(__rdtsc() - start < 1000000000);

    }
    void prime_probe::prime(const prime_probe::prime_probe_buffer& pb) {
    // tomato: prime 
    }

    void prime_probe::probe(const prime_probe::prime_probe_buffer& pb, prime_probe::results& res) {
    // tomato: probe
    }

    void prime_probe::clearAll(prime_probe::cache_sets &set){
    // tomato: we want to replace the primed set for time measurement.
    }
    void prime_probe::probeSet(const prime_probe::prime_probe_buffer& pb, prime_probe::results& res, uint32_t set_to_probe){
    // tomato: prime a specific set
    }
    uint64_t prime_probe::getTime(const prime_probe::results& res, uint32_t set){
        return res.times[set];
    }
