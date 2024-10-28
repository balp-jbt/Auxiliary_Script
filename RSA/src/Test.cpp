#include "RSA.h"
void div_test() {
    pair<BigInt*, BigInt*> qr;

    // ------------------  [group1] 64  ------------------
    // base_t a1 = 0xefecdac3ffffffff;
    // base_t a3 = 0x65acee1affffffff;
    // base_t a5 = 0xffec998affffffff;

    // base_t b1 = 0xffffffffffffffff;
    // base_t b2 = 0xffffffffffffff;
    // vector<base_t> data_a = {a5, a3, a1}, data_b = {b1, b2};

    // ------------------  [group1] 32  ------------------
    // base_t a1 = 0xefecdac3;
    // base_t a2 = 0xffffffff;
    // base_t a3 = 0x65acee1a;
    // base_t a4 = 0xfffffffff;
    // base_t a5 = 0xffec998a;
    // base_t a6 = 0xffffffff;

    // base_t b1 = 0xffffffff;
    // base_t b2 = 0xffffffff;
    // vector<base_t> data_a = {a6, a5, a4, a3, a2, a1}, data_b = {b2, b1, b2, b1};
    
    // base_t a1 = 0x80000000;
    // base_t a2 = 0x00000000;

    // base_t b1 = 0xffffffff;
    // base_t b2 = 0xffffff;

    // vector<base_t> data_a = {a2, a2, a2, a2, a1}, data_b = {b1, b2};

    // -------------------THIS HAS MINUS, 32 --------------------
    // base_t a1 = 0x161af693;
    // base_t a2 = 0x435a3cbf;
    // base_t a3 = 0x56ad2b9e;
    // base_t a4 = 0xa1ef7ae5;
    // base_t a5 = 0x5315cc0b;
    // base_t a6 = 0x2630b129;
    // base_t a7 = 0x9b374adb;
    // base_t a8 = 0x2e313c79;
    
    // base_t b1 = 0xe7fa53be;
    // base_t b2 = 0xcf7850b3;
    // base_t b3 = 0x640762c1;

    // vector<base_t> data_a = {a8, a7, a6, a5, a4, a3, a2, a1}, data_b = {b3, b2, b1};

    // -------------------THIS HAS MINUS, 64 --------------------
    // base_t a1 = 0x161af693435a3cbf;
    // base_t a3 = 0x56ad2b9ea1ef7ae5;
    // base_t a5 = 0x5315cc0b2630b129;
    // base_t a7 = 0x9b374adb2e313c79;
    
    // base_t b1 = 0xe7fa53be;
    // base_t b2 = 0xcf7850b3640762c1;
    // vector<base_t> data_a = {a7, a5, a3, a1}, data_b = {b2, b1};

    // --------------------SEEMS HARD -----------------
    // base_t a0 = 0x86b3bf42e8bb590b;
    // base_t a1 = 0x9912a23cfeb67a55;
    // base_t a2 = 0x726047bf13ef7f20;
    // base_t a3 = 0x81a4b692e0a5ff97;
    // base_t a4 = 0x72293911ac77c085;
    // base_t a5 = 0xff102f04c0a12648;
    // base_t a6 = 0x0f4ce29f59c974bf;
    // base_t a7 = 0xcdbb028f6b198f5c;
    // base_t a8 = 0xb3648f547ab14cdc;
    // base_t a9 = 0x06cd5d891d736d32;
    // base_t a10 = 0x56dc8358685d3205;
    // base_t a11 = 0x2f2444044926c0bf;
    // base_t a12 = 0x88634946c9adfd48;
    // base_t a13 = 0xc2519ebe3127dde1;
    // base_t a14 = 0x94dabc1d13f1158c;
    // base_t a15 = 0x12b624aa17f3d62f;


    // base_t b0 = 0xd4aae7333654d857;
    // base_t b1 = 0xa2972a7219159880;

    // vector<base_t> data_a = {a15, a14, a13, a12, a11, a10, a9, a8, a7, a6, a5, a4, a3, a2, a1, a0};
    // vector<base_t> data_b = {b1, b0};
    // BigInt a(&data_a);
    // BigInt b(&data_b);
    // a.print_plain("a is\n");
    // b.print_plain("b is\n");
    // qr = a.div(&b);
    // qr.first->print_plain("Here is the anwser1  q \n");
    // qr.second->print_plain("Here is the anwser1  r \n");

    // // FASTMOD, very hard ---PROBLEM---
    base_t a0 = 0x86b3bf42e8bb590b;
    base_t a1 = 0x9912a23cfeb67a55;
    base_t a2 = 0x726047bf13ef7f20;
    base_t a3 = 0x81a4b692e0a5ff97;
    base_t a4 = 0x72293911ac77c085;
    base_t a5 = 0xff102f04c0a12648;
    base_t a6 = 0x0f4ce29f59c974bf;
    base_t a7 = 0xcdbb028f6b198f5c;
    base_t a8 = 0xb3648f547ab14cdc;
    base_t a9 = 0x06cd5d891d736d32;
    base_t a10 = 0x56dc8358685d3205;
    base_t a11 = 0x2f2444044926c0bf;
    base_t a12 = 0x88634946c9adfd48;
    base_t a13 = 0xc2519ebe3127dde1;
    base_t a14 = 0x94dabc1d13f1158c;
    base_t a15 = 0x12b624aa17f3d62f;


    base_t b0 = 0xd4aae7333654d857;
    base_t b1 = 0xa2972a7219159880;


    base_t modulo = 0xffffffffffffffff;
    vector<base_t> data_a = {a15, a14, a13, a12, a11, a10, a9, a8, a7, a6, a5, a4, a3, a2, a1, a0};
    vector<base_t> data_b = {b1, b0, b1, b0};
    vector<base_t> data_c = {modulo, modulo};
    BigInt a(&data_a);
    BigInt b(&data_b);
    BigInt c(&data_c);
    BigInt* res = a.modular_exponentiation(&b, &c);
    res->print_plain("FAST MOD\n");

    // BigInt a = BigInt((base_t)100);
    // BigInt b = BigInt((base_t)100);
    // BigInt c = BigInt((base_t)67);
    // BigInt* res = a.modular_exponentiation(&b, &c);
    // res->print_plain("FAST MOD\n");
}


int main () {
    div_test();
    return 0;
}