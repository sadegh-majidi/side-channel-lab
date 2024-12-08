#ifndef __FLUSH_RELOAD_H__
#define __FLUSH_RELOAD_H__

#include <cstdint>
#include <x86intrin.h>
namespace flush_reload {
inline void flush(const void *addr) {
        asm volatile("clflush (%0)" :: "r"(addr));
}

inline uint64_t timeMemoryAccess(const void *addr){
    uint64_t time1, time2, msl;
    _mm_mfence();
    _mm_lfence();
    time1 = __rdtsc();
    _mm_lfence();
    int temp = *((int *) addr);
    _mm_lfence();
    time2 = __rdtsc();
    _mm_lfence();
    msl = time2 - time1;
    return msl;
}

uint64_t miss_threshold;
inline void registerMissThreshold(uint64_t time){
    miss_threshold = time;
}
inline bool isMiss(uint64_t time){
    return time >= miss_threshold;
}

}

#endif // __FLUSH_RELOAD_H__
