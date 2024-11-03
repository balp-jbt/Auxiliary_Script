#include "RSA.h"

int main (int argc, char*argv[]) {
    argc-- , argv++;
    Config config(argc, argv);
    RSA::generate_key(768);
    RSA::encrypt("./id_rsa.pub", "./plain_context", "./cipher_context");
    RSA::decrypt("./id_rsa.pub", "./id_rsa", "./cipher_context", "./decrypt_context");
    return 0;
}