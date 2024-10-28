#include "RSA.h"

RSA::RSA() {
    size_t prime_e = RSA_PUBLIC_EXPONENT;
    this -> prime_e = BigInt((base_t)prime_e);
}

void RSA::generate_key(string path, size_t bit_len) {
    BigInt* p = BigInt::generate_prime(bit_len / 2);
    BigInt* q = BigInt::generate_prime(bit_len / 2);
    while (p->compare(q) == EQ) {
        q = BigInt::generate_prime(bit_len);
    }
    BigInt* n = p -> mult(q);
    BigInt* p_minus_one = p->sub(&BigInt::big_one);
    BigInt* q_minus_one = q->sub(&BigInt::big_one);
    BigInt* phi_n = p_minus_one->mult(q_minus_one);
    // BigInt* d = BigInt::extend_gcd(&BigInt::big_e, phi_n);
    std::ofstream target_file(path);
    if (! target_file) {
        throw runtime_error("Error in opening target file!\n");
    }
    target_file << "RSA PUBLIC KEY" << endl;
    target_file << RSA_PUBLIC_EXPONENT << endl;
    target_file << n->to_hex() << endl;
    target_file.close();
    return;
}

void RSA::encrypt_data(BigInt key, string in_path, string out_path) {
    return;
}

void RSA::decrypt_data(BigInt key, string in_path, string out_path) {
    return;
}

