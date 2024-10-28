#include "RSA.h"

int main () {
    RSA::generate_key("./id_rsa.pub", 768);
    return 0;
}