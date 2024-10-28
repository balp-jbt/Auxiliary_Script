// TODO: remove all #ifdef for debug

#ifndef __CONFIG__
#define __CONFIG__

// RSA Options
#define MILLER_RABIN_THRESHOLD 20

// BigInt
// #define BASE_WIDTH 32
// #define BASE_MASK UINT32_MAX
#define BASE_WIDTH 64
#define BASE_MASK UINT64_MAX
#define BASE_HIGH_MASK ((base_t)1 << (BASE_WIDTH-1))
#define BASE ((mult_t)1 << BASE_WIDTH)

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
#include <stdexcept>

#ifdef DEBUG_MODE_ON
#include <iostream>
#include <iomanip>
#endif

using std::vector;
using std::string;
using std::pair;
using std::make_pair;
using std::runtime_error;

#ifdef DEBUG_MODE_ON
using std::cout;
using std::cin;
using std::endl;
using std::hex;
using std::dec;
using std::setw;
using std::setfill;
#endif

// typedef uint64_t mult_t;
// typedef uint32_t base_t;
typedef uint64_t base_t;
typedef __uint128_t mult_t;


inline std::string uint128ToString(__uint128_t value) {
    std::string result;
    while (value) {
        result.insert(result.begin(), '0' + (value % 10));
        value /= 10;
    }
    return result.empty() ? "0" : result;
}

inline std::ostream& operator<<(std::ostream& os, __uint128_t value) {
    os << uint128ToString(value);
    return os;
}

#endif