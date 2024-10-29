#include "RSA.h"

RSA::RSA() {}

void RSA::generate_key(size_t bit_len, string path_pub, string path_priv) {
    BigInt* p = BigInt::generate_prime(bit_len / 2);
    BigInt* q = BigInt::generate_prime(bit_len / 2);
    while (p->compare(q) == EQ) {
        q = BigInt::generate_prime(bit_len);
    }
    BigInt* n = p -> mult(q);
    BigInt* p_minus_one = p->sub(&BigInt::big_one);
    BigInt* q_minus_one = q->sub(&BigInt::big_one);
    BigInt* phi_n = p_minus_one->mult(q_minus_one);
    pair<BigInt*, pair<BigInt*, BigInt*>> gcd_res = BigInt::extend_gcd(phi_n, &BigInt::big_e);
    BigInt* d = gcd_res.second.second;

    std::ofstream private_key_file(path_priv);
    if (! private_key_file) {
        throw runtime_error("Error in opening target file!\n");
    }
    // private_key_file << "RSA PRIVATE KEY" << endl;
    private_key_file << d->to_hex() << endl;
    private_key_file.close();

    std::ofstream public_key_file(path_pub);
    if (! public_key_file) {
        throw runtime_error("Error in opening target file!\n");
    }
    // public_key_file << "RSA PUBLIC KEY" << endl;
    public_key_file << RSA_PUBLIC_EXPONENT << endl;
    public_key_file << n->to_hex() << endl;
    // public_key_file << "[prime1] " << p->to_hex() << endl;
    // public_key_file << "[prime2] " << q->to_hex() << endl;
    // public_key_file << "[phi_n] " << phi_n->to_hex() << endl;
    public_key_file.close();
    return;
}

string handle_unit(size_t key_len, BigInt* key, BigInt* target) {
    #ifdef DEBUG_MODE_ON
        assert(target->data->size() * BASE_WIDTH % key_len == 0);
    #endif
    // BigInt* res = ;
}

void RSA::handle(string key_path, string data_path, string out_path) {
    return;
}

