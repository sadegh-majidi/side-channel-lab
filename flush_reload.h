#ifndef __FLUSH_RELOAD_H__
#define __FLUSH_RELOAD_H__

#include <cstdint>
#include <x86intrin.h>
namespace flush_reload {
inline void flush(const void *addr) {
        asm volatile(/*tomato*/);
}

inline uint64_t timeMemoryAccess(const void *addr){
    //tomato: return time for access.
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
