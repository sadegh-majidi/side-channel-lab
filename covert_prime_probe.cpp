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
#include <assert.h>
#include "prime_probe.h"
#include "helper.h"

// You can define global variables if you feel the need.

uint64_t timing_threshold = 0;

void calibrate_threshold(prime_probe::prime_probe_buffer& pb, prime_probe::cache_sets& sett, prime_probe::results& res) {
    prime_probe::prime(pb);
    prime_probe::probe(pb, res);
    uint64_t sum = 0;
    uint64_t miss_thresh = 0, hit_thresh = 0;
    for (int i = 0; i < 8; i++) {
        for (uint32_t set = 0; set < pb.number_of_sets; set++) {
            sum += res.times[set];
        }
    }
    hit_thresh = sum / pb.number_of_sets;
    printf("hit average time: %lu\n", hit_thresh);

    sum = 0;
    for (int i = 0; i < 8; i++) {
        for (uint32_t set = 0; set < pb.number_of_sets; set++) {
            prime_probe::clearAll(sett);
            prime_probe::probeSet(pb, res, set);
            sum += res.times[set];
        }
        
    }
    miss_thresh = sum / (pb.number_of_sets * 8);
    printf("miss average time: %lu\n", miss_thresh);

    timing_threshold = (miss_thresh + hit_thresh) / 2;
    printf("-> setting probe time threshold to: %lu\n", timing_threshold);
}


//tomato: fill in the next four functions
void postBit(prime_probe::prime_probe_buffer& pb, prime_probe::cache_sets& set, uint8_t bit){
    if (bit == 1) {
        prime_probe::prime(pb);
    } else {
        prime_probe::clearAll(set);
    }
}
void postLength(prime_probe::prime_probe_buffer& pb, prime_probe::cache_sets& set, int length){
    prime_probe::prime(pb);

    for (uint32_t i = length; i < pb.number_of_sets; i++) {
        void* current = set.eviction_sets[i];
        while (current) {
            current = *((void**)current);
        }
    }
}
uint8_t recvBit(prime_probe::prime_probe_buffer& pb,prime_probe::results& res){
    prime_probe::probe(pb, res);
    uint64_t total_time = 0;
    for (uint32_t set = 0; set < pb.number_of_sets; set++) {
        total_time += res.times[set];
    }
    return (total_time / pb.number_of_sets < timing_threshold) ? 1 : 0;
}
uint8_t recvLength(prime_probe::prime_probe_buffer& pb,prime_probe::results& res){
    uint8_t length = 0;

    for (uint32_t set = 0; set < pb.number_of_sets; set++) {
        prime_probe::probeSet(pb, res, set);
        if (prime_probe::getTime(res, set) < timing_threshold) {
            length++;
        }
    }

    return length;
}
inline uint8_t getBit(char src, uint8_t bit_position){
    return (src >> bit_position) & 1;
}

inline uint8_t setBit(char src, uint8_t bit_position){
    return (src | (1 << bit_position));
}

int process1(){
    volatile int x = 135;
    volatile int y = 12;
    for (int i= 0; i <1000; i++){
        x = x * y;
    }
}

int process2(){
    volatile int x = 135;
    volatile int y = 12;
    for (int i= 0; i <1000; i++){
        x = x * y;
    }
}

int main(int argc, char *argv[]) {
  // We will have number_of_sets, number_of_ways, number_of_blocks_per_way, block_size, key
  // specified as arguments.
  //
  if(argc != 4){
    printf("Usage: ./cov_prime_probe <number_of_sets> <number_of_ways> <block_size>\n");
    return 1;
  }

  uint32_t number_of_sets = atoi(argv[1]);
  uint32_t number_of_ways = atoi(argv[2]);
  uint32_t block_size = atoi(argv[3]);

  prime_probe::prime_probe_buffer pb;
  prime_probe::results res;
  prime_probe::cache_sets set;
  prime_probe::setup(pb, res, set, number_of_sets, number_of_ways, block_size);

  calibrate_threshold(pb, set, res);
// tomato: you can introduce global variables and keep timing information to facilitate the communication functions.

  // process 1 gets a message and posts it.
  std::string message;
  std::getline(std::cin, message);
  // process 2 keeps track of the message length and the message itself.
  size_t length = 0;
  std::string received_message;
  std::stringstream received_message_stream;
  {
    process1();
    postLength(pb,set, message.length());
  }
  {
      process2();
      length = recvLength(pb, res);
  }
  for(int i = 0; i < length; i++){
      uint8_t character = 0;
      for(int j = 0; j < 8; j++){
          uint8_t bit = 0;
          {
              process1();
              bit = getBit(message[i], j);
              postBit(pb,set, bit);
          }
          {
              process2();
              bit = recvBit(pb, res);
              character = setBit(character, bit);
          }
      }
      {
          process1();
      }
      {
            process2();
            received_message_stream << character;
      }
  }
  received_message = received_message_stream.str();
  std::cout << "Received message: " << received_message << std::endl;


}
