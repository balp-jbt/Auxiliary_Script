#ifndef __CONFIG__
#define __CONFIG__

// BigInt
#define BASE_WIDTH 64
#define BASE_MASK UINT64_MAX

// Debug_Config
#define DEBUG_MODE_ON 

#include <stdint.h>
#include <vector>
#include <string>
#include <cassert>
#include <cstring>

#ifdef DEBUG_MODE_ON
#include <iostream>
#include <iomanip>
#endif

using std::vector;
using std::string;

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