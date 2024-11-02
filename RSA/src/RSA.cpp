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

    ofstream private_key_file(path_priv);
    if (! private_key_file) {
        throw runtime_error("Error in opening target file!\n");
    }
    // private_key_file << "RSA PRIVATE KEY" << endl;
    private_key_file << d->to_hex() << endl;
    private_key_file.close();

    ofstream public_key_file(path_pub);
    if (! public_key_file) {
        throw runtime_error("Error in opening target file!\n");
    }
    // public_key_file << "RSA PUBLIC KEY" << endl;
    public_key_file << BigInt::big_e.to_hex() << endl;
    public_key_file << n->to_hex() << endl;
    // uncomment this to get primes and phi
    public_key_file << "[prime1] " << p->to_hex() << endl;
    public_key_file << "[prime2] " << q->to_hex() << endl;
    public_key_file << "[phi_n] " << phi_n->to_hex() << endl;
    public_key_file.close();
    return;
}

BigInt* RSA::handle_unit(BigInt* key_e, BigInt* key_p, BigInt* target) {
    BigInt* res = target->modular_exponentiation(key_e, key_p);
    return res;
}

void RSA::encrypt(string pub_key_path, string plain_context_path, string out_path) {
    ifstream pub_key_file(pub_key_path);
     if (! pub_key_file) {
        throw runtime_error("Error in opening target file!\n");
    }

    ifstream plain_context_file(plain_context_path);
    if (! plain_context_file) {
        throw runtime_error("Error in opening target file!\n");
    }

    ofstream res_file(out_path);
    if (! res_file) {
        throw runtime_error("Error in opening target file!\n");
    }

    string key_1, key_2;
    getline(pub_key_file, key_1);
    getline(pub_key_file, key_2);
    pub_key_file.close();

    BigInt* pub_key_e = new BigInt(key_1);
    BigInt* pub_key_p = new BigInt(key_2);
    size_t key_len = key_2.size();
    size_t unit_byte_size = (key_len / 8) - 11;
    vector<BigInt*> res;
    ostringstream oss;
    oss << plain_context_file.rdbuf();
    string target_string = oss.str();
    for (size_t i = 0; i < target_string.size(); i += unit_byte_size) {
        std::string unit_str = target_string.substr(i, unit_byte_size);
        ostringstream hexStream;
        for (unsigned char c : unit_str) {
            hexStream << hex << setw(2) << setfill('0') << static_cast<int>(c);
        }
        string hex_target = hexStream.str();
        BigInt* target = new BigInt(hex_target);
        
        BigInt* encrypted_unit =  handle_unit(pub_key_e, pub_key_p, target);

        res.push_back(encrypted_unit);
    }

    for (BigInt* unit: res) {
        res_file << unit->to_hex() << endl;
    }
    res_file.close();
    delete pub_key_e;
    delete pub_key_p;
    return;
}

void RSA::decrypt(string pub_key_path, string priv_key_path, string cipher_context_path, string out_path) {
    ifstream pub_key_file(pub_key_path);
     if (! pub_key_file) {
        throw runtime_error("Error in opening target file!\n");
    }
    ifstream priv_key_file(priv_key_path);
    if (! priv_key_file) {
        throw runtime_error("Error in opening target file!\n");
    }

    ifstream cipher_context_file(cipher_context_path);
    if (! cipher_context_file) {
        throw runtime_error("Error in opening target file!\n");
    }

    ofstream out_file(out_path);
    if (! out_file) {
        throw runtime_error("Error in opening target file!\n");
    }

    string key_1, key_2, key_3;
    getline(pub_key_file, key_1);
    getline(pub_key_file, key_2);
    getline(priv_key_file, key_3);

    BigInt* pub_key_p = new BigInt(key_2);
    BigInt* priv_key = new BigInt(key_3);
    
    pub_key_file.close();
    priv_key_file.close();

    string line_cipher;
    BigInt* unit_cipher;
    BigInt* unit_res;
    ostringstream res;
    while (getline(cipher_context_file, line_cipher)) {
        unit_cipher = new BigInt(line_cipher);
        unit_res = handle_unit(priv_key, pub_key_p, unit_cipher);
        string inner_res = unit_res->to_hex();
        for (size_t i = 0; i < inner_res.size(); i += 2) {
            string byteString = inner_res.substr(i, 2);
            char byte = static_cast<char>(strtol(byteString.c_str(), nullptr, 16));
            res << byte;
        }
    }
    
    delete pub_key_p;
    delete priv_key;
    cipher_context_file.close();
    out_file << res.str();
    out_file.close();
    return;
}