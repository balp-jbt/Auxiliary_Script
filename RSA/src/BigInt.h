#ifndef __BIGINT__
#define __BIGINT__

#include "Config.h"

class BigInt{
public:
    vector<base_t> *data;
    bool is_positive;

    BigInt(bool is_positive = true);                     
    BigInt(vector<base_t> *data, bool is_positive = true);
    BigInt(base_t &data, bool is_positive = true);        
    BigInt(BigInt* other);

    ~BigInt();

    // Fundamental operations (BigInt.cpp)
    int compare(BigInt* other);

    size_t get_bit_len();

    //pre-condition: this and other should be positive and this.size >= other.size
    // NOTE: if is_in_place, result needs to be deleted manually
    BigInt* add(BigInt *other, bool is_in_place = false);

    //pre-condition: this should be >= other
    // NOTE: if is_in_place, result needs to be deleted manually
    BigInt* sub(BigInt *other, bool is_in_place = false);

    //pre-condition: this and other should be positive and this.size >= other.size
    // NOTE: result needs to be deleted manually
    BigInt* mult(BigInt *other);

    BigInt* l_shift(size_t n, bool is_in_place = false);

    BigInt* r_shift(size_t n, bool is_in_place = false);

    BigInt* div(BigInt *other);

    BigInt operator%(const BigInt &other) const;

    void print() const;

    BigInt* fast_power(BigInt* exponet);

    void remove_leading_zero();

#ifdef DEBUG_MODE_ON
    void print_plain(string auxiliary_text);
#endif

    // Other support operations (BigInt_RSA_Operation.cpp)
    BigInt(string *data);

    BigInt* modularExponentiation(BigInt* pow, BigInt* base);

    // NOTE: result needs to be deleted manually
    BigInt* generate_random_given_len(size_t but_len);

    BigInt* generate_prime(size_t bit_len);

    BigInt extend_gcd(BigInt p, BigInt q);

    bool miller_rabin_test(BigInt* target, int threshold=MILLER_RABIN_THRESHOLD);

    static inline base_t _zero = 0, _one = 1, _two = 2;
    static BigInt big_zero, big_one, big_two;
};
#endif