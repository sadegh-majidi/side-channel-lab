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
#include "flush_reload.h"
#include "helper.h"

// You can define global variables if you feel the need.

void* cache_lines[255];

//tomato: fill in the next five functions
void setupCacheLines(){
}

void postChar(char src){

}
void postLength(uint8_t length){

}
char recvChar(){

}
uint8_t recvLength(){

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
    printf("Usage: ./cov_flush_reload <number_of_sets> <number_of_ways> <block_size>\n");
    return 1;
  }

  uint32_t number_of_sets = atoi(argv[1]);
  uint32_t number_of_ways = atoi(argv[2]);
  uint32_t block_size = atoi(argv[3]);
  std::cout << "Number of sets: " << number_of_sets << std::endl;
  std::cout << "Number of ways: " << number_of_ways << std::endl;
  std::cout << "Block size: " << block_size << std::endl;


  // process 1 gets a message and posts it.
  std::string message;
  std::getline(std::cin, message);
  // process 2 keeps track of the message length and the message itself.
  uint8_t length = 0;
  std::string received_message;
  std::stringstream received_message_stream;
  {
    process1();
    postLength(set, message.length());
  }
  {
      process2();
      length = recvLength(pb, res);
  }
    for(int i = 0; i < length; i++){
        char character;
        {
            process1();
            postChar( message[i]);
        }
        {
            process2();
            character = recvChar();
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
