#include "BigInt.h"

BigInt BigInt::big_zero((base_t)0);
BigInt BigInt::big_one((base_t)1);
BigInt BigInt::big_two((base_t)2);

BigInt::BigInt(bool is_positive) {
    this -> is_positive = is_positive;
    this -> data = new vector<base_t>;
    this-> data -> push_back(0);
}


BigInt::BigInt(vector<base_t> *data, bool is_positive) {
    this -> is_positive = is_positive;
    this -> data = new vector<base_t>;
    for (size_t i = 0; i < data->size(); i++) {
        (*this->data).push_back((*data)[i]);
    }
}

BigInt::BigInt(base_t data, bool is_positive) {
    this -> is_positive = is_positive;
    base_t unit = data & BASE_MASK;
    this -> data = new vector<base_t>;
    (this -> data) -> push_back(unit);
}

BigInt::BigInt(BigInt* other) {
    this->is_positive = other->is_positive;
    this->data = new vector<base_t>;
    this->data->resize(other->data->size());
    for (size_t i = 0; i < other->data->size(); i++) {
        (*this->data)[i] = (*other->data)[i];
    }
}

BigInt::~BigInt() {
    delete this -> data;
}

int BigInt::compare(BigInt* other) {
    if (this->data->size() > other->data->size()) {
        return GT;
    }
    if (this->data->size() < other->data->size()) {
        return LT;
    }

    for (size_t i = 0; i < this->data->size(); i++) {
        if ((*this->data)[i] > (*other->data)[i]) {
            return GT;
        }
        if ((*this->data)[i] < (*other->data)[i]) {
            return LT;
        }
    }
    return EQ;
}

size_t BigInt::get_bit_len() {
    size_t res, offset = 0, mask = BASE_HIGH_MASK;
    while (true) {
        if ((*this->data)[0] & mask) {
            break;
        }
        offset++;
        mask = (mask >> 1);
        #ifdef DEBUG_MODE_ON
            assert(mask != 0);
        #endif
    }
    res = BASE_WIDTH - offset;
    res += (this->data->size() - 1) * BASE_WIDTH;
    return res;
}

BigInt* BigInt::add(BigInt *other, bool is_in_place) {
    
    #ifdef DEBUG_MODE_ON
        assert(this->is_positive && other->is_positive);
        assert(this->data->size() >= other->data->size());
    #endif

    BigInt* res;
    if (! is_in_place) {
        res = new BigInt(this);
    } else {
        res = this;
    }
    res->data->resize(this->data->size()+1);
    
    bool carry = false;
    base_t unit;
    for (size_t i = 0; i < (res->data->size())-1; i++) {
        unit = (*res->data)[i];
        (*res->data)[i] += (*other->data)[i] + carry;
        carry = ((carry && (*res->data)[i] <= unit) || (!carry && (*res->data)[i] < unit));
    }

    for (size_t i = other->data->size(); i < (res->data->size())-1; i++) {
        unit = (*res->data)[i];
        (*res->data)[i] += carry;
         carry = ((carry && (*res->data)[i] <= unit) || (!carry && (*res->data)[i] < unit));
    }

    if (carry) {
        (*res->data)[(res->data->size())-1] = 1;
    }

    res->remove_leading_zero();
    return res;
}

BigInt* BigInt::sub(BigInt *other, bool is_in_place) {
    #ifdef DEBUG_MODE_ON
        assert(this->compare(other) != LT);
    #endif

    BigInt *res;
    if (! is_in_place) {
        res = new BigInt(this);
    } else {
        res = this;
    }
    
    bool borrow = false;
    base_t unit;
    for (size_t i = 0; i < other->data->size(); i++) {
        unit = (*this->data)[i] - (*other->data)[i] - borrow;
        borrow = ((borrow && (*this->data)[i] <= (*other->data)[i]) ||
                    (!borrow && (*this->data)[i] < (*other->data)[i]));
        (*res->data)[i] = unit;
    }
    for (size_t i = other->data->size(); i < this->data->size(); i++) {
        unit = (*this->data)[i] - borrow;
        borrow = (borrow && (*this->data)[i] == 0);
        (*res->data)[i] = unit;
    }
    res->remove_leading_zero();
    return res;
}

void BigInt::remove_leading_zero() {
    while (this->data->size() > 1 && (*this->data)[this->data->size()-1] == 0) {
        this->data->pop_back();
    }
}

// TODO: Use Karatsuba algorithm as an option
BigInt* BigInt::mult(BigInt* other) {

    #ifdef DEBUG_MODE_ON
        assert(this->is_positive && other->is_positive);
        assert(this->data->size() >= other->data->size());
    #endif

    BigInt* res = new BigInt();
    res->data->resize(this->data->size() + other->data->size());

    mult_t unit;
    base_t carry;
    for (size_t i = 0; i < this->data->size(); i++) {
        carry = 0;
        for (size_t j = 0; j < other->data->size(); j++) {
            unit =  (mult_t)(*this->data)[i] * (mult_t)(*other->data)[j] + (*res->data)[i+j] + carry;
            (*res->data)[i+j] = (base_t)unit;
            carry = (unit >> BASE_WIDTH);
        }
        unit = (*res->data)[i+other->data->size()] + carry;
        #ifdef DEBUG_MODE_ON
            assert((unit >> BASE_WIDTH) == 0);
        #endif
        (*res->data)[i+other->data->size()] = (base_t)unit;
    }

    res->remove_leading_zero();
    return res;
}

BigInt* BigInt::l_shift(size_t n, bool is_in_place) {
    size_t unit_shift = n / BASE_WIDTH;
    size_t part_shift = n % BASE_WIDTH;
    BigInt* res;
    if (is_in_place) {
        res = this;
    } else {
        res = new BigInt();
    }
    res->data->resize(this->data->size()+ unit_shift + 1);
    if (part_shift) {
        (*res->data)[res->data->size()-1] = ((*this->data)[res->data->size()-unit_shift-2] >> (BASE_WIDTH-part_shift));
    } else {
        (*res->data)[res->data->size()-1] = 0;
    }
    
    for (size_t i = res->data->size()-2; ; i--) {
        if (i > unit_shift) {
            if (part_shift) {
                (*res->data)[i] = ((*this->data)[i-unit_shift] << part_shift) | ((*this->data)[i-unit_shift-1] >> (BASE_WIDTH-part_shift));
            } else {
                (*res->data)[i] = ((*this->data)[i-unit_shift] << part_shift);
            }
            
        } else if (i == unit_shift) {
            (*res->data)[i] = ((*this->data)[i-unit_shift] << part_shift);
        } else {
            (*res->data)[i] = 0;
        }
        if (!i) {
            break;
        }
    }
    res->remove_leading_zero();
    return res;
}

BigInt* BigInt::r_shift(size_t n, bool is_in_place) {
    size_t unit_shift = n / BASE_WIDTH;
    size_t part_shift = n % BASE_WIDTH;

    BigInt* res;
    if (is_in_place) {
        res = this;
    } else {
        res = new BigInt();
    }

    res->data->resize(this->data->size());
    for (size_t i = 0; i < res->data->size()-unit_shift; i++) {
        if (this->data->size() > i+1+unit_shift) {
            if (part_shift) {
                (*res->data)[i] = (((*this->data)[i+unit_shift]) >> part_shift) | (((*this->data)[i+unit_shift+1]) << (BASE_WIDTH-part_shift));
            } else {
                (*res->data)[i] = (((*this->data)[i+unit_shift]) >> part_shift);
            }
            
        } else {
            (*res->data)[i] = (((*this->data)[i+unit_shift]) >> part_shift);
        }
        
    }
    
    if (is_in_place) {
        this->data->resize(this->data->size() - unit_shift);
    }

    res->remove_leading_zero();
    return res;
}

// Knuth Algorithm D (TAOCP)
// See: https://stackoverflow.com/questions/60479571/is-there-a-bug-in-algorithm-4-3-1d-of-knuths-taocp?newreg=b087a46374d54c05a6ec133f5e71f73d
//      https://ridiculousfish.com/blog/posts/labor-of-division-episode-iv.html
pair<BigInt*, BigInt*> BigInt::div(BigInt* other) {
    if (other->compare(&big_zero) == EQ) {
        throw runtime_error("Dividened zero error!");
    }


    if (this->compare(other) == LT) {
        BigInt* res = new BigInt((base_t) 0);
        BigInt* remainder = new BigInt(other);
        return make_pair(res, remainder);
    }

    
    if (other->data->size() == 1) {
        BigInt* quotient = new BigInt(this);
        mult_t unit = 0;
        for (size_t i = this->data->size() - 1; ; i--) {
            unit = (unit << BASE_WIDTH) + (*this->data)[i];
            (*quotient->data)[i] = unit / (*other->data)[0];
            unit = unit % (*other->data)[0];
            cout << "unit=  " << hex << (base_t)unit << endl << dec;
            if (i == 0) {
                break;
            }
        }
        quotient->remove_leading_zero();
        cout << "unit=  " << hex << (base_t)unit << endl << dec;
        BigInt* remainder = new BigInt((base_t)unit);
        remainder->remove_leading_zero();
        return make_pair(quotient, remainder);
    }


    vector<base_t> quotient_data;
    BigInt divisor = BigInt(other);
    BigInt* dividend = new BigInt(this);

    // [D1] Normalize
    size_t l_distance = 0;
    base_t bit_mask = BASE_HIGH_MASK;
    while (! (bit_mask & (*divisor.data)[divisor.data->size() - 1])) {
        bit_mask = (bit_mask >> 1);
        l_distance++;
        #ifdef DEBUG_MODE_ON
            assert(l_distance < BASE_WIDTH);
        #endif
    }
    cout << "[1] l_distance= " << l_distance << endl; 
    divisor.l_shift(l_distance, true);
    dividend->l_shift(l_distance, true);
    //guarante dividened.size() > divisor.size()
    dividend->data->push_back(0);
    cout << "[---]" << dividend->data->size() << "  " << divisor.data -> size() << endl;
    quotient_data.resize(dividend->data->size() - divisor.data->size() + 1);

    // [D2] Initialize j
    size_t cur_index = dividend->data->size() - divisor.data->size() - 1;
    mult_t divisor_high = (*divisor.data)[divisor.data->size()-1];
    mult_t divisor_sec_high = (*divisor.data)[divisor.data->size()-2];
    
    while(true) {
        cout << "[0] " << "i = " << cur_index << endl;
        // [D3] calculate \hat{q} i.e. estimated q
        mult_t unit_dividened;
        mult_t quotient_estimate;
        mult_t remainder_estimate;
        size_t cur_dividened_index = cur_index + divisor.data->size();
        unit_dividened = ((mult_t)((mult_t) (*dividend->data)[cur_dividened_index] << BASE_WIDTH) +
                    (mult_t)(*dividend->data)[cur_dividened_index-1]);
        quotient_estimate = (unit_dividened) / divisor_high;
        std::cout << "[2] estimate_q= " << std::hex << quotient_estimate << std::dec << "\n";
        remainder_estimate = (unit_dividened) % divisor_high;
        while (quotient_estimate == BASE || 
            ((remainder_estimate << BASE_WIDTH) + (mult_t)(*dividend->data)[cur_dividened_index-2]
                < (quotient_estimate * divisor_sec_high))) {
                    cout << "HELLO?" << endl;
                    quotient_estimate--;
                    remainder_estimate += divisor_high;
                    if (remainder_estimate >= BASE) {
                        break;
                    }
        }

        // [D4] Multiply and substract
        base_t res_unit; 
        mult_t subtrahend_unit = 0;
        bool borrow = false;
        for (size_t divisor_index = 0; divisor_index <= divisor.data->size(); divisor_index++) {
            if (divisor_index < divisor.data->size()) {
                subtrahend_unit = subtrahend_unit + quotient_estimate * (*divisor.data)[divisor_index];
            }
            res_unit = (*dividend->data)[cur_index + divisor_index] - borrow - (base_t)subtrahend_unit;
            borrow = ((borrow && (*dividend->data)[cur_index + divisor_index] <= (base_t)subtrahend_unit) ||
                        (!borrow && (*dividend->data)[cur_index + divisor_index] < (base_t)subtrahend_unit));
            (*dividend->data)[cur_index+divisor_index] = res_unit;
            cout << "[4] set index " << cur_index+divisor_index << " to " << res_unit << endl;
            subtrahend_unit = (subtrahend_unit >> BASE_WIDTH);
        }

        // [D5] Test remainder
        if (borrow) {
            // [D6] Add back
            quotient_estimate--;
            bool carry = false;
            res_unit = 0;
            for (size_t divisor_index = 0; divisor_index < divisor.data->size(); divisor_index++) {
                res_unit = (*dividend->data)[divisor_index + cur_index] + (*divisor.data)[divisor_index] + carry;
                carry = ((carry && res_unit <= (*dividend->data)[divisor_index + cur_index]) ||
                            (!carry && res_unit < (*dividend->data)[divisor_index + cur_index]));
                (*dividend->data)[divisor_index + cur_index] = res_unit;
            }

            #ifdef DEBUG_MODE_ON
                assert(carry = true); //seems overflow is neccessary because there was a borrow in substract
                //assert((*dividend->data)[divisor.data->size() -1 + cur_index] == 0);
            #endif

        }
        quotient_data[cur_index] = (base_t)quotient_estimate;
       

        // [D7] Loop on j
        if (cur_index == 0) {
            break;
        }
        cur_index--;

    }

    // [D8] Unnormalize (Note: dividened is reamainder)
    dividend->data->resize(divisor.data->size());
    dividend->remove_leading_zero();
    dividend->r_shift(l_distance, true);
    dividend->remove_leading_zero();
    BigInt* quotient = new BigInt(&quotient_data);
    quotient->remove_leading_zero();
    return make_pair(quotient, dividend);
}

BigInt* BigInt::fast_power(BigInt* exponet) {
    
}

#ifdef DEBUG_MODE_ON
void BigInt::print_plain(string auxiliary_text) {
    cout << auxiliary_text;
    for (size_t i = this->data->size() - 1; ; i--) {
        cout << hex << std::setw(BASE_WIDTH / 4) << std::setfill('0') << (*this->data)[i] << dec;
        if (i == 0) {
            break;
        }
        // cout << ",";
    }
    cout << endl;
}
#endif

