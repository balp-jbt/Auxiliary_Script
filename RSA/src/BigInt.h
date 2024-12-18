#ifndef __BIGINT__
#define __BIGINT__

#include "Config.h"

class BigInt{
public:
    vector<base_t> *data;
    bool is_positive;

    BigInt(bool is_positive = true);                     
    BigInt(vector<base_t> *data, bool is_positive = true);
    BigInt(base_t data, bool is_positive = true);        
    BigInt(BigInt* other);
    
    //pre-condition: \forall i, hen_str[i] \in {0...9, a...f}
    BigInt(string& hex_str, bool is_positivee = true);

    ~BigInt();

    // Fundamental operations (BigInt.cpp)
    int compare(BigInt* other);

    size_t get_bit_len();

    // NOTE: if is_in_place false, result needs to be deleted manually
    BigInt* add(BigInt *other, bool is_in_place = false);

    //pre-condition: this should be >= other
    // NOTE: if is_in_place false, result needs to be deleted manually
    BigInt* sub(BigInt *other, bool is_in_place = false);

    //pre-condition: this and other should be positive
    // NOTE: result needs to be deleted manually
    BigInt* mult(BigInt *other);

    // NOTE: if is_in_place, result needs to be deleted manually
    BigInt* l_shift(size_t n, bool is_in_place = false);

    // NOTE: if is_in_place, result needs to be deleted manually
    BigInt* r_shift(size_t n, bool is_in_place = false);

    // NOTE: result needs to be deleted manually
    pair<BigInt*, BigInt*> div(BigInt *other);

    void remove_leading_zero();

    string to_hex();
    
#ifdef DEBUG_MODE_ON
    void print_plain(string auxiliary_text);
#endif

    // Other support operations (BigInt_RSA_Operation.cpp)

    BigInt* modular_exponentiation(BigInt* pow, BigInt* base);

    // NOTE: result needs to be deleted manually
    static BigInt* generate_random_given_len(size_t but_len);

    static BigInt* generate_prime(size_t bit_len);

    static bool miller_rabin_test(BigInt* target, int threshold=MILLER_RABIN_THRESHOLD);

    static pair<BigInt*, pair<BigInt*, BigInt*>> extend_gcd(BigInt* p, BigInt* q);

    static BigInt big_zero, big_one, big_two, big_e;
    static vector<BigInt*> small_prime;
    

};
#endif