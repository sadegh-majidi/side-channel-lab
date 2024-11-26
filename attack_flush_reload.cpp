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
#include <vector>
#include <algorithm>
#include "aes.h"
#include "flush_reload.h"
#include "helper.h"


int main(int argc, char *argv[]) {
  // We will have number_of_sets, number_of_ways, number_of_blocks_per_way, block_size, key
  // specified as arguments.
  //
  if(argc != 3){
    printf("Usage: ./flush_reload <key> <block_size>\n");
    return 1;
  }

  char * arg1 = strdup(argv[1]);
  uint32_t block_size = atoi(argv[2]);

  std::ofstream aes_file("flush_reload_output.txt");

  initRandomSampler();
  aes_t key;
  std::string key_str(arg1,34);
  std::stringstream ss;
  ss << "flush_reload_" << key_str << ".txt";
  std::ofstream aes_file_keys(ss.str());

  parseHex(key_str, key);

  std::string input_str, output_str;
  AES_KEY aeskey;
  private_AES_set_encrypt_key(key, 128, &aeskey);

  // get T-Tables
  const void* te0 = getTe0();
  const void* te1 = getTe1();
  const void* te2 = getTe2();
  const void* te3 = getTe3();
  

  aes_t input, output;
  std::cout << "Flushed line load cost: ";
  uint64_t sum = 0, miss_threshold,miss_time,hit_time;
  for(int i = 0; i < 10000; i++){
    //tomato find out how much a miss takes
  }
  sum=0;
  
  for(int i = 0; i < 10000; i++){
    //tomato find out how much a hit takes
  }
  //tomato find a good miss_threshold from hit and miss times.
  flush_reload::registerMissThreshold(miss_threshold);
  // tomato: you can introduce new structures to infer the key from the input and output.
  // The possible keys will be stored in the file aes_file_keys.
  
  std::vector<int> offset_miss_count;
  offset_miss_count.resize(256*4/block_size, 0);
  for(int j = 0; j < 10000; j++){
    char* te0_current = (char*)te0;
    for(int i = 0; i < 256*4; i += /*tomato*/){ 
      makeRandomAesInputExcept1ByteOfZeros(input, 0);
      flush_reload::flush((void*)te0_current);
      AES_encrypt(input, output, &aeskey);
      uint64_t time = flush_reload::timeMemoryAccess((void*)te0_current);
      if(flush_reload::isMiss(time)){
        offset_miss_count[i/block_size]++;
      }
      te0_current += block_size;
      aes_file << "Offset: " << std::hex << "0x" << i/4 << ", Time: " << std::dec << time<< std::endl; 
    }
  }
  uint64_t no_miss_offset, offset_size = 256*4/block_size;
  for (int i = 0; i < offset_size; i++){
    if(!offset_miss_count[i]){
      no_miss_offset = i*block_size/4;
    }
  }
  uint64_t biggest_possible = no_miss_offset + block_size/4;
  aes_file_keys << "Possible: 0x" << std::hex << no_miss_offset << std::endl;
  // tomato: if you find an exact solution for the byte you can write it by
  // aes_file_keys << "Exact: 0x:" << std::hex << exact_first_byte << std::endl;
}
