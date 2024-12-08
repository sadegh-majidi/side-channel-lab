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
#define CACHE_ALIGNEMENT 64
void prime_probe::setup(prime_probe::prime_probe_buffer& pb, prime_probe::results& res,prime_probe::cache_sets& set,
                        uint32_t number_of_sets, uint32_t number_of_ways, uint32_t block_size){
    pb.number_of_sets = number_of_sets;
    pb.number_of_ways = number_of_ways;

    pb.eviction_sets = (void**)malloc(number_of_sets * sizeof(void *));
    res.times = (uint64_t*)aligned_alloc(CACHE_ALIGNEMENT, number_of_sets * sizeof(uint64_t));
    // We want to make sure we dont use the same page for multiple ways
	void* pointers = aligned_alloc(CACHE_ALIGNEMENT, number_of_ways * PAGE_SIZE);
    void** pointer_array = (void**)malloc(number_of_ways * sizeof(void *));
	// A cache set so that we can measure the time it takes to access the cache set
	void* underlying_for_eviction_sets = aligned_alloc(CACHE_ALIGNEMENT, number_of_ways * PAGE_SIZE);
	set.eviction_sets = (void**)malloc(number_of_sets * sizeof(void *));
    set.number_of_sets = number_of_sets;
    set.number_of_ways = number_of_ways;

	for(int cache_set=0; cache_set < number_of_sets; cache_set++){
		for (int i = 0; i < number_of_ways; i++) pointer_array[i] = (void *)((uintptr_t) pointers + cache_set * block_size + i * PAGE_SIZE);
                // We want to make a linked list for the eviction sets.
                // tomato: if you want you can also introduce a shuffle for the pointer array
		for(int j = 0; j < number_of_ways; j++){
		    *((void**)pointer_array[j]) = pointer_array[(j + 1) % number_of_ways];
		}
        *((void**)pointer_array[number_of_ways - 1]) = nullptr;
		pb.eviction_sets[cache_set] = pointer_array[0];
		for (int i = 0; i < number_of_ways; i++) pointer_array[i] = (void *)((uintptr_t)underlying_for_eviction_sets + cache_set * block_size + i * PAGE_SIZE);
		for(int j = 0; j < number_of_ways; j++){
		    *((void**)pointer_array[j]) = pointer_array[(j + 1) % number_of_ways];
		}
        *((void**)pointer_array[number_of_ways - 1]) = nullptr;
		set.eviction_sets[cache_set] = pointer_array[0];
        assert(set.eviction_sets[cache_set] != pb.eviction_sets[cache_set]);
	}

	free(pointer_array);


	uint64_t start = __rdtsc();
	while(__rdtsc() - start < 1000000000);

    }
    void prime_probe::prime(const prime_probe::prime_probe_buffer& pb) {
        for (int set = 0; set < pb.number_of_sets; set++) {
            void* current = pb.eviction_sets[set];
            while (current) {
                current = *((void**)current);
            }
        }
    }

    void prime_probe::probe(const prime_probe::prime_probe_buffer& pb, prime_probe::results& res) {
        for (uint32_t set = 0; set < pb.number_of_sets; set++) {
            _mm_mfence();
            _mm_lfence();
            uint64_t start = __rdtsc();
            _mm_lfence();
            void* current = pb.eviction_sets[set];
            while (current) {
                current = *((void**)current);
            }
            _mm_lfence();
            uint64_t end = __rdtsc();
            _mm_lfence();
            res.times[set] = end - start;
        }
    }

    void prime_probe::clearAll(prime_probe::cache_sets &set){
        for (int i = 0; i < set.number_of_sets; i++) {
            void* current = set.eviction_sets[i];
            while (current) {
                current = *((void**)current);
            }
        }
    }
    void prime_probe::probeSet(const prime_probe::prime_probe_buffer& pb, prime_probe::results& res, uint32_t set_to_probe){
        assert(set_to_probe < pb.number_of_sets);
        _mm_mfence();
        _mm_lfence();
        uint64_t start = __rdtsc();
        _mm_lfence();
        void* current = pb.eviction_sets[set_to_probe];
        while (current) {
            current = *((void**)current);
        }
        _mm_lfence();
        uint64_t end = __rdtsc();
        _mm_lfence();
        res.times[set_to_probe] = end - start;
    }
    uint64_t prime_probe::getTime(const prime_probe::results& res, uint32_t set){
        return res.times[set];
    }
