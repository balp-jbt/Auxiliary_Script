#ifndef __RSA__
#define __RSA__

#include "BigInt.h"

class RSA{

public:
    size_t key_len;

    RSA();

    static void generate_key(size_t bit_len, string path_pub = "./id_rsa.pub", string path_priv = "./id_rsa");

    static string handle_unit(size_t key_len, BigInt* key, BigInt* target);

    static void handle(string key_path, string data_path, string out_path);
    
};

#endif