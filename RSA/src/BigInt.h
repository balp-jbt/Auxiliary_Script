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

    ~BigInt();

    // Fundamental operations (BigInt.cpp)

    //pre-condition: this and other should be positive and this.size >= other.size
    BigInt* add(BigInt *other, bool is_in_place = false);

    //pre-condition: this and other should be positive and this.size >= other.size
    BigInt* mult(BigInt *other);

    BigInt operator*(const BigInt &other);
    BigInt operator/(const BigInt &other) const;
    BigInt operator%(const BigInt &other) const;

    bool operator<(const BigInt &other) const;
    bool operator>(const BigInt &other) const;
    bool operator==(const BigInt &other) const;
    bool operator!=(const BigInt &other) const;

    void print() const;

    BigInt fast_power(BigInt exponet);

    void remove_leading_zero();

#ifdef DEBUG_MODE_ON
    void print_plain(string auxiliary_text);
#endif

    // Other support operations (BigInt_RSA_Operation.cpp)
    BigInt(string *data);
    
    BigInt generate_prime(size_t bit_len);

    BigInt extend_gcd(BigInt p, BigInt q);

    bool miller_rabin(BigInt target, int k);

    
};

#endif