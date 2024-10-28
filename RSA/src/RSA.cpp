#include "RSA.h"

RSA::RSA() {
    size_t prime_e = RSA_PUBLIC_EXPONENT;
    this -> prime_e = BigInt((base_t)prime_e);
}

void RSA::generate_key(string path) {
    return;
}

void RSA::encrypt_data(BigInt key, string in_path, string out_path) {
    return;
}

void RSA::decrypt_data(BigInt key, string in_path, string out_path) {
    return;
}

