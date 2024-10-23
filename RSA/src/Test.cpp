#include "RSA.h"

void add_test() {

    base_t a1 = 0xefecdac3ffffffff;
    base_t a2 = 0x65acee1affffffff;
    base_t a3 = 0xffec998affffffff;

    base_t b1 = 0xffffffffffffffff;
    base_t b2 = 0xffffffffffffffff;

    vector<base_t> data_a = {a3, a2, a1}, data_b = {b2, b1};

    BigInt a(&data_a);
    a.print_plain("Here is the a\n");

    BigInt b(&data_b);
    b.print_plain("Here is the b\n");

    BigInt* c = a.add(&b);
    c->print_plain("Here is the anwser1\n");

    base_t d1 = 0xffffffffffffffff;
    base_t e1 = 0xffffffffffffffff;
    BigInt d(d1);
    BigInt e(e1);
    BigInt* f = d.add(&e);
    f->print_plain("Here is the anwser2\n");
}

void mult_test() {
    base_t w1 = 0x13;
    base_t w2 = 0x13;
    BigInt o1(w1);
    BigInt o2(w2);
    BigInt* o = o1.mult(&o2);
    o->print_plain("Here is the anwser0\n");


    base_t a1 = 0xefecdac3ffffffff;
    base_t a2 = 0x65acee1affffffff;
    base_t a3 = 0xffec998affffffff;

    base_t b1 = 0xffffffffffffffff;
    base_t b2 = 0xffffffffffffffff;

    vector<base_t> data_a = {a3, a2, a1}, data_b = {b2, b1};

    BigInt a(&data_a);
    BigInt b(&data_b);

    BigInt* c = a.mult(&b);
    c->print_plain("Here is the anwser1\n");

    base_t d1 = 0xffffffffffffffff;
    base_t e1 = 0xffffffffffffffff;
    BigInt d(d1);
    BigInt e(e1);
    BigInt* f = d.mult(&e);
    f->print_plain("Here is the anwser2\n");

    vector<base_t> data_z = {b1, b1};
    vector<base_t> data_zz = {b1, b1};
    BigInt z(&data_z);
    BigInt zz(&data_zz);
    BigInt* z_res = z.mult(&zz);
    z_res->print_plain("Here is the anwser3\n");
}

int main () {
    add_test();
    cout << "GAP" << endl << endl << endl;
    mult_test();
    return 0;
}