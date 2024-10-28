#ifndef __RSA__
#define __RSA__

#include "BigInt.h"

class RSA{

private: 
    BigInt prime_p;
    BigInt prime_q;
    BigInt private_key;

public:
    BigInt public_key;
    BigInt prime_e;

    RSA();

    static void generate_key(string path, size_t bit_len);

    static void encrypt_data(BigInt key, string in_path, string out_path);

    static void decrypt_data(BigInt key, string in_path, string out_path);    

    
};

#endif