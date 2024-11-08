// TODO: remove all #ifdef for debug
// TODO: check for delete for every new, especially in fast_expotienal_mod

#ifndef __CONFIG__
#define __CONFIG__

// RSA Options
#define MILLER_RABIN_THRESHOLD 10

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

#define RSA_PUBLIC_EXPONENT 65537 // Choose e = 65537 as the standard requires

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
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <chrono>
#include <iostream>
#include <iomanip>

using std::vector;
using std::string;
using std::pair;
using std::make_pair;
using std::runtime_error;
using std::ostringstream;
using std::cout;
using std::cin;
using std::endl;
using std::hex;
using std::dec;
using std::setw;
using std::setfill;
using std::ofstream;
using std::ifstream;
using std::getline;
using std::unordered_map;
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::to_string;

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


class Config {
public:

    static unordered_map<string, string> options;

    static void load_args(int argc, char* argv[]);

    static bool check_opt(string target1, string target2);

    static string get_opt(string target1, string target2, bool force);
};
#endif