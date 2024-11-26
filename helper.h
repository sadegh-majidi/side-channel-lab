#ifndef __HELPER_H__
#define __HELPER_H__
#include <random>
#include <string>

typedef uint8_t aes_t[16];
void parseHex(std::string& src, aes_t dst){
    // get rid of 0x
    std::string hex = src.substr(2);
    // every two characters is a byte
    for (int i = 0; i < 16; i++){
        std::string byte = hex.substr(i*2, 2);
        dst[i] = (uint8_t)strtol(byte.c_str(), NULL, 16);
    }
}

void parseBytes(aes_t src, std::string& dst){
    dst.clear();
    // go over each byte and convert to hex representation
    // and append to the string
    for (int i = 0; i < 16; i++){
        char byte[3]={};
        sprintf(byte, "%02x", src[i]);
        dst.append(byte);
    }
}
struct randomSampler{
    std::uniform_int_distribution<int> distribution;
    std::default_random_engine generator;
};
static randomSampler sampler;
void initRandomSampler(){
    sampler.distribution = std::uniform_int_distribution<int>(0, 255);
    sampler.generator = std::default_random_engine();
}

void makeRandomAesInput(aes_t dst){
    // generate random bytes
    // discrete uniform distribution of chars
    for (int i = 0; i < 16; i++){
        dst[i] = (uint8_t)sampler.distribution(sampler.generator);
    }
}

void makeRandomAesInputExcept1ByteOfZeros(aes_t dst,int zeros_location){
    for (int i = 0; i < 16; i++){
        dst[i] = (uint8_t)sampler.distribution(sampler.generator);
    }
        dst[zeros_location] = 0;
    
}
void makeRandomAesInputExcept1ByteOfValue(aes_t dst,int location,uint8_t value){
    for (int i = 0; i < 16; i++){
        dst[i] = (uint8_t)sampler.distribution(sampler.generator);
    }
    dst[location] = value;
}
#endif // __HELPER_H__
