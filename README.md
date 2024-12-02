# This is the Purdue Principles of Computer Architecture Sidechannel Attack Assignment

## Contents

* [General Rules](#general-rules)

* [Flush & Reload](#flush-reload)

* [Prime & Probe](#prime-probe)

* [Covert-Channel](#covert-channel)

* [Aes](#aes)

* [Grading](#grading)

## Overview

This homework requires a linux x86 machine, if you are a CS person we recommend using one of the servers, if you do not have access to an x86 machine contact us, if you have an x86 windows PC you can use WSL. We have two different attacks with two different strategies to implement them, the attack strategies are:

1) Flush & Reload
2) Prime & Probe

The attacks are:

1) Covert-Channel
2) Aes side-channel

Details are given in their respective sections in this document.

## General Rules

We have several files you will need to modify for this homework, but please make sure you don't modify:

* aes_lib.cpp
* aes.h
* helper.h

For those files that you need to modify,  hints are provided by comments that contain the word "tomato". These points in the code are the ones that need modification, while you can do a restructuring around these points, for the side-channel attack only given code can access the key, for the covert-channel attack any direct communication (i.e. writing to shared memory and reading) is not allowed.

## Flush & Reload

For flush & reload we want you to fill in functions in the flush_reload.h file:

* flush
* timeMemoryAccess

*flush* implements the flush of a specific cache line including an address. *timeMemoryAccess* reads the given address and times the access. When you need to classify something as a miss or not you will need to determine a good miss threshold and register it with *registerMissThreshold*. You can then use *isMiss* to figure out if an access needs to be classified as a miss.

## Prime & Probe

Next, you need to fill in the following functions in prime_probe.cpp file:

* setup
* prime
* probe
* clearAll
* probeSet

The *setup* function allocates and arranges prime and probe buffer that we use, it also fills in another buffer to be used in *clearAll* to replace the primed cache lines, we use this to get timing difference to infer if a cache line was replaced. *prime* primes the cache with the "pb" structure. *probe* loads "pb" to get timing information. *clearAll* is the same as prime but will load from another structure to evict primed cache lines. *probeSet* probes a specific set.

## Covert-Channel

In this task there are two different files to manipulate:

* covert_flush_reload.cpp
* covert_prime_probe.cpp

For flush and reload we want to implement:

* setupCacheLines
* postChar
* postLength
* recvChar
* recvLength

*setupCacheLines* sets up a cache line per array element that we will use to communicate with. *postChar* will flush a specific line to communicate a character, make sure you can differentiate the flushed line after this function. *postLength* will flush a specific line the same way *postChar* does, it is written as a different function to make reading the communication code easier. *recvChar* will time and figure out which line was sent. *recvLength* is also the same as *recvChar*.

For prime and probe we want to implement:

* postBit
* postLength
* recvBit
* recvLength

*postBit* posts a bit, you can define the operation yourself, it needs to differentiate between 0 and 1. *postLength* can be used to send a length you want to be able to send lengths in the range: \[0, number_of_sets\]. The recv functions will differentiate what was sent by the post functions.

## Aes

In this task there are two different files to manipulate:

* attack_flush_reload.cpp
* attack_prime_probe.cpp

The aim is to recover the first byte of the aes key. The aes key will be used to xor the input and produce bytes, these bytes are used to index a table. Check the aes_lib.cpp:795, we want to check accesses to this table by giving an input byte of zero at the first position. If there is an access to the specific line or the cache set is used more than average, we can infer the offset to the table that is accessed by aes_lib.cpp:795. This will give us a range of possible bytes for the first byte of aes_key. If you are able to infer the whole key by incorprating the other table accesses (you can also correlate with other keys), you can also output the result as "Exact: ${first_byte}". 

## Grading

2% extra credit each
1) Covert-Channel Flush & Reload
2) Covert-Channel Prime & Probe
3) Aes Flush & Reload
4) Aes Prime & Probe
5) Aes Flush & Reload (Exact Byte)
6) Aes Prime & Probe (Exact Byte)
