#include "BigInt.h"

BigInt::BigInt(string *data) {

}

BigInt* BigInt::modular_exponentiation(BigInt* exponent, BigInt* modulo) {
    BigInt* mid_pow_res = new BigInt(this);
    BigInt* res = new BigInt(base_t(1));
    pair<BigInt*, BigInt*> div_res;
    div_res= mid_pow_res->div(modulo);
    mid_pow_res = div_res.second;
    mid_pow_res->print_plain("[a] \n");
    for (size_t unit_i = 0; unit_i < exponent->data->size(); unit_i++) {
        base_t bit_mask = 1;
        size_t part_i = 0;
        while(true) {
            cout << "<" << unit_i * BASE_WIDTH + part_i << ">" << endl;
            if ((*exponent->data)[unit_i] & bit_mask) {
                res->print_plain("[ans add] before:\n");
                res = res->mult(mid_pow_res);\
                res->print_plain("[ans add] mid:\n");
                div_res = res->div(modulo);
                res = div_res.second;
                res->print_plain("[ans add] after:\n");
            }
            part_i++;
            if (part_i > BASE_WIDTH) {
                break;
            }
            mid_pow_res = mid_pow_res->mult(mid_pow_res);
            mid_pow_res->print_plain("[a1]\n");
            div_res = mid_pow_res->div(modulo);
            mid_pow_res = div_res.second;
            mid_pow_res->print_plain("[a2]\n");
            bit_mask = (bit_mask << 1);
        }
    }
    return res;
}

BigInt* BigInt::generate_random_given_len(size_t bit_len) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<long long> distrib_binary(0, 1);
    std::uniform_int_distribution<long long> distrib_unit((base_t)0x0, (base_t)BASE_MASK);
    size_t unit_len = bit_len / BASE_WIDTH;
    size_t part_len = bit_len % BASE_WIDTH;
    if (!part_len) {
        unit_len--;
        part_len = BASE_WIDTH - 1;
    }

    vector<base_t> init_data;
    init_data.resize(unit_len + 1);
    for (size_t i = 1; i <= unit_len; i++) {
        init_data[i] = distrib_unit(gen);
    }
    base_t part = 0;
    for (size_t i = 0; i < part_len-1; i++) {
        part = (part << 1) | distrib_binary(gen);
    }
    part = (part << 1) | 1;
    init_data[0] = part;
    BigInt* target = new BigInt(&init_data);
    target->remove_leading_zero();
    return target;
}

BigInt* BigInt::generate_prime(size_t bit_len) {
    BigInt* target = generate_random_given_len(bit_len);

    #ifdef DEBUG_MODE_ON
        assert((*target->data)[0] & 1);
    #endif

    while (target->compare(&big_one) != GT) {
        delete target;
        target = generate_random_given_len(bit_len);
            #ifdef DEBUG_MODE_ON
                assert((*target->data)[0] & 1);
            #endif
    }

    while (true) {
        if (miller_rabin_test(target)) {
            return target;
        }
        target->add(&big_two, true); // TODO: check whether the left 'target' could be ignored
    }
}


bool BigInt::miller_rabin_test(BigInt* target, int threshold) {
    
    // [1] target = 2^s * d;
    size_t s = 0;
    BigInt _d(target);
    BigInt* d = &_d;
    BigInt _target_minus_one(target);
    BigInt* target_minus_one = &_target_minus_one;
    

    (*target_minus_one->data)[0] = (*target_minus_one->data)[0] & (BASE_MASK - 1);
    (*d->data)[0] = (*d->data)[0] & (BASE_MASK - 1);

    size_t cur_unit = 0, cur_part = 1;
    while (true) {
        if (((*d->data)[cur_unit] >> cur_part) & 1) {
            break;
        }
        s++;
        cur_part++;
        if (cur_part == 63) {
            cur_part = 0;
            cur_unit++;
        }
        if (cur_unit == d->data->size() - 1 && cur_part == BASE_WIDTH - 1) {
            break;
        }
    }

    d -> r_shift(s, true); // TODO: check whether the left 'target' could be ignored

    // [2] test-loop
    int test_time = 0;
    size_t target_len = target -> get_bit_len();
    while (test_time < threshold) {
        test_time++;
        // [2-1] choose random a \in (1, n-1) 
        BigInt* a = generate_random_given_len(target_len);
        while (a->compare(target_minus_one) != GT) {
            delete a;
            a = generate_random_given_len(target_len);
        }
        // [2-2] Check a^d mod n, if get 1 then true, pass this round
        

        // [2-3] for i \in [0 ... s-1], check a^{(2^i)d} mod n, if get n-1, pass this round

        
        
        return false;
    }

    return true;



}