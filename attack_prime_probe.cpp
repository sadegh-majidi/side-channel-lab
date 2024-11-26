#include <cstdint>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <cstring>
#include <iostream>
#include <sys/mman.h>
#include <sys/types.h>
#include <x86intrin.h>
#include <vector>
#include <algorithm>
#include "aes.h"
#include "prime_probe.h"
#include "helper.h"

bool isMiss(uint64_t time, uint64_t hit_time, uint64_t miss_time, uint32_t number_of_ways,uint32_t average_usage){
  // tomato: return true if the access indicates our test accessed a set more than usual.
}

uint32_t hitCount(uint64_t time, uint64_t hit_time, uint64_t miss_time, uint32_t number_of_ways){
  // tomato: return the number of hits per a set with access time of "time"
}


int main(int argc, char *argv[]) {
  // We will have number_of_sets, number_of_ways, number_of_blocks_per_way, block_size, key
  // specified as arguments.
  //
  if(argc != 5){
    printf("Usage: ./prime_probe <key> <number_of_sets> <number_of_ways> <block_size>\n");
    return 1;
  }

  char * arg1 = strdup(argv[1]);
  uint32_t number_of_sets = atoi(argv[2]);
  uint32_t number_of_ways = atoi(argv[3]);
  uint32_t block_size = atoi(argv[4]);

  std::ofstream aes_file("prime_probe_output.txt");

  aes_t key;
  std::string key_str(arg1,34);
  std::stringstream ss;
  ss << "prime_probe_" << key_str << ".txt";
  std::ofstream aes_file_keys(ss.str());
  initRandomSampler();




  parseHex(key_str, key);

  std::string input_str, output_str;
  AES_KEY aeskey;
  private_AES_set_encrypt_key(key, 128, &aeskey);
  aes_t input, output;
  prime_probe::prime_probe_buffer pb;
  prime_probe::results res;
  prime_probe::cache_sets set;
  prime_probe::setup(pb, res, set, number_of_sets, number_of_ways, block_size);
  // for convinience we calculate the cache_set of te0

  uintptr_t te0 = (uintptr_t)getTe0();
  // each set is block_size apart.
  uint64_t te0_set = (te0 / block_size) % number_of_sets;
  uint64_t te0_last = te0_set + (256*4/block_size);


  // tomato: You can figure out how much time misses will take
  // You can read the information to figure out possible keys. Output to aes_file_keys.
  uint64_t sum = 0;
  for(int i = 0;i < 10000; i++){
    //tomato: evict and measure
  }
  uint64_t all_ways_evicted = sum/10000;
  uint64_t miss_time = all_ways_evicted/number_of_ways;
  sum = 0;
  for(int i = 0;i < 10000; i++){
    //tomato: do not evict only probe.
  }
  uint64_t all_ways_not_evicted = sum/10000;
  uint64_t hit_time = all_ways_not_evicted/number_of_ways;
  std::cout << "Hit time: " << hit_time << std::endl;
  std::cout << "Miss time: " << miss_time << std::endl;
  int invoke = 16; // tomato: find a good amount to run the encryption to replace the cache lines
  std::vector<int> offset_access_count;
  std::vector<float> average_hit_counts;
  offset_access_count.resize(256*4/block_size, 0);
  average_hit_counts.resize(256*4/block_size, 0);
  for(int j = 0; j < 100000; j++){
      makeRandomAesInput(input);
    prime_probe::prime(pb);
    for(int i = 0; i < invoke; i++){
      AES_encrypt(input, output, &aeskey);
    }
    prime_probe::probe(pb, res);
    uint64_t offset = 0; // this is an index to a 4 byte per element array
    for(uint64_t check=te0_set; check < te0_last; check++){
      average_hit_counts[offset*4/block_size]+= (float)hitCount(res.times[check],hit_time,miss_time,number_of_ways)/100000.0;
    }
  }
  for(int j = 0; j < 100000; j++){
      makeRandomAesInputExcept1ByteOfZeros(input, 0);
    prime_probe::prime(pb);
    for(int i = 0; i < invoke; i++){
      AES_encrypt(input, output, &aeskey);
    }
    prime_probe::probe(pb, res);
    uint64_t offset = 0; // this is an index to a 4 byte per element array
    for(uint64_t check=te0_set; check < te0_last; check++){
      if(!isMiss(res.times[check],hit_time,miss_time,number_of_ways, average_hit_counts[offset*4/block_size])){
        //tomato: there is an update here that we need, to keep track of the access count per the sets we check.
      }
      offset += block_size/4; // each set increments the index this much
      aes_file << "Offset: " << std::hex << "0x" << offset << ", Time: " << std::dec << res.times[check] << std::endl; 
    }
  }
  // Report the offsets that give the minimum amount of misses.
  int min_miss = *std::min_element(offset_access_count.begin(),
      offset_access_count.end());
  for(int i =0; i < 256*4/block_size; i++){
    if(offset_access_count[i] == min_miss){
      aes_file_keys << "Possible: " << i*block_size/4<< std::endl;
    }
  }
  // tomato: if you find an exact solution for the byte you can write it by
  // aes_file_keys << "Exact: 0x:" << std::hex << exact_first_byte << std::endl;
}
