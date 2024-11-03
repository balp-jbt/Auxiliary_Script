#ifndef __RSA__
#define __RSA__

#include "BigInt.h"

class RSA{

public:

    static BigInt* handle_unit(BigInt* key_e, BigInt* key_p, BigInt* target);

    // mode = true for encrypt by pub_key, otherwise by priv_key(sign)
    static void encrypt(bool mode, string pub_key_path, string priv_key_path, string plain_context_path, string out_path);

    // mode = true for decrypt by priv_key, otherwise by pub_key(verify)
    static void decrypt(bool mode, string pub_key_path, string priv_key_path, string cipher_context_path, string out_path);
    
    static void generate_key(size_t bit_len, string path_pub, string path_priv);
};

#endif