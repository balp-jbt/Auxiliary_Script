// TODO: remove all #ifdef for debug

#ifndef __CONFIG__
#define __CONFIG__

// RSA Options
#define MILLER_RABIN_THRESHOLD 20

// BigInt
#define BASE_WIDTH 64
#define BASE_MASK UINT64_MAX
#define BASE_HIGH_MASK (1 << (BASE_WIDTH-1))

#define EQ 0
#define GT 1
#define LT 2


// Debug_Config
#define DEBUG_MODE_ON 

#include <stdint.h>
#include <vector>
#include <string>
#include <cassert>
#include <cstring>
#include <utility>
#include <random>

#ifdef DEBUG_MODE_ON
#include <iostream>
#include <iomanip>
#endif

using std::vector;
using std::string;
using std::pair;

#ifdef DEBUG_MODE_ON
using std::cout;
using std::cin;
using std::endl;
using std::hex;
using std::dec;
using std::setw;
using std::setfill;
#endif

typedef uint64_t base_t;
typedef __uint128_t mult_t;

#endif