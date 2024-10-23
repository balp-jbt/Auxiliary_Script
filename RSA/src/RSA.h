#ifndef __RSA__
#define __RSA__

#include "BigInt.h"


#define RSA_PUBLIC_EXPONENT 65537 // Choose e = 65537 as the standard requires

class RSA{

private: 
    BigInt prime_p;
    BigInt prime_q;
    BigInt private_key;

public:
    BigInt public_key;
    BigInt prime_e;

    RSA();

    void generate_key(string path);

    void encrypt_data(BigInt key, string in_path, string out_path);

    void decrypt_data(BigInt key, string in_path, string out_path);    


};

#endif