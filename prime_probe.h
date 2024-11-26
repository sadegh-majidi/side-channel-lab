#ifndef __PRIME_PROBE_H__
#define __PRIME_PROBE_H__

#include <cstdint>
namespace prime_probe {
    struct prime_probe_buffer{
        uint32_t number_of_sets;
        uint32_t number_of_ways;
        void **eviction_sets;
    };
    struct results{
        uint64_t *times;
    };
    struct cache_sets{
        uint32_t number_of_sets;
        uint32_t number_of_ways;
        void **eviction_sets;
    };
    void setup(prime_probe_buffer& pb,results& res,cache_sets& set, uint32_t number_of_sets, uint32_t number_of_ways, uint32_t block_size);
    void prime(const prime_probe_buffer& pb);
    void probe(const prime_probe_buffer& pb, results& res);
    void clearAll(cache_sets& set);
    void probeSet(const prime_probe_buffer& pb, results& res, uint32_t set_to_probe);
    uint64_t getTime(const results& res, uint32_t set);

}


#endif // __PRIME_PROBE_H__
