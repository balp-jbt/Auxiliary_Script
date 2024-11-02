#ifndef __RSA__
#define __RSA__

#include "BigInt.h"

class RSA{

public:

    RSA();

    static BigInt* handle_unit(BigInt* key_e, BigInt* key_p, BigInt* target);

    static void encrypt(string pub_key_path = "./id_rsa.pub", string plain_context_path = "./plain_context", string out_path = "./cipher_context");

    static void decrypt(string pub_key_path = "./id_rsa.pub", string priv_key_path = "./id_rsa", string cipher_context_path = "./cipher_context", string out_path = "./decrypt_context");
    
    static void generate_key(size_t bit_len = 1024, string path_pub = "./id_rsa.pub", string path_priv = "./id_rsa");
    
};

#endif