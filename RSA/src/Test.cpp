#include "RSA.h"

int main () {
    RSA::generate_key(1024);
    RSA::encrypt("./id_rsa.pub", "./plain_context", "./cipher_context");
    RSA::decrypt("./id_rsa.pub", "./id_rsa", "./cipher_context", "./decrypt_context");
    return 0;
}