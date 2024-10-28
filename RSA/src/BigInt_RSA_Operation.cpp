#include "BigInt.h"

BigInt BigInt::big_e = new BigInt((base_t)RSA_PUBLIC_EXPONENT);


vector<BigInt*> BigInt::small_prime = [] {
    std::vector<BigInt*> vec;
    vector<base_t> _small_prime = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
        53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109,
        113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179,
        181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241,
        251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313,
        317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389,
        397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461,
        463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547,
        557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617,
        619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691,
        701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773,
        787, 797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859,
        863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947,
        953, 967, 971, 977, 983, 991, 997
    };

    for (base_t prime : _small_prime) {
        vec.push_back(new BigInt(prime));
    }
    return vec;
}();



BigInt::BigInt(string *data) {

}

BigInt* BigInt::modular_exponentiation(BigInt* exponent, BigInt* modulo) {
    BigInt* mid_pow_res = new BigInt(this);
    BigInt* res = new BigInt(base_t(1));
    pair<BigInt*, BigInt*> div_res;
    div_res= mid_pow_res->div(modulo);
    mid_pow_res = div_res.second;
    for (size_t unit_i = 0; unit_i < exponent->data->size(); unit_i++) {
        base_t bit_mask = 1;
        size_t part_i = 0;
        while(true) {
            if ((*exponent->data)[unit_i] & bit_mask) {
                res = res->mult(mid_pow_res);
                div_res = res->div(modulo);
                res = div_res.second;
            }
            part_i++;
            if (part_i > BASE_WIDTH) {
                break;
            }
            mid_pow_res = mid_pow_res->mult(mid_pow_res);
            div_res = mid_pow_res->div(modulo);
            mid_pow_res = div_res.second;
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

    while (target->compare(&big_two) != GT) {
        delete target;
        target = generate_random_given_len(bit_len);
            #ifdef DEBUG_MODE_ON
                assert((*target->data)[0] & 1);
            #endif
    }

    while (true) {
        bool pass = true;
        
        for (BigInt* small_prime: BigInt::small_prime) {
            pair<BigInt*, BigInt*> res = target -> div(small_prime);
            if (res.second->compare(&big_zero) == EQ) {
                pass = false;
                break;
            }
        }
        if (pass && BigInt::miller_rabin_test(target)) {
            return target;
        }
        target = target->add(&big_two);
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
        if (cur_part == BASE_WIDTH - 1) {
            cur_part = 0;
            cur_unit++;
        }
        if (cur_unit == d->data->size() - 1 && cur_part == BASE_WIDTH - 1) {
            break;
        }
    }

    d -> r_shift(s, true);

    // [2] test-loop
    int test_time = 0;
    size_t target_len = target -> get_bit_len();
    while (test_time < threshold) {
        test_time++;
        // [2-1] choose random a \in (1, n-1) 
        BigInt* a = generate_random_given_len(target_len);
        while (a->compare(target_minus_one) == GT) {
            delete a;
            a = generate_random_given_len(target_len);
        }
        // [2-2] Check a^d mod n, if get 1 then true, pass this round
        BigInt* res = a->modular_exponentiation(d, target);
        if (res->compare(&big_one) == EQ) {
            test_time++;
            continue;
        }
        
        // [2-3] for i \in [0 ... s-1], check a^{(2^i)d} mod n, if get n-1, pass this round
        bool pass = false;
        for (size_t i = 0; i < s; i++) {
            res = res->modular_exponentiation(&big_two, target);
            if (res == target_minus_one) {
                pass = true;
                break;
            }
        }
        if (! pass) {
            return false;
        }
        test_time++;
        continue;
    }

    return true;
}