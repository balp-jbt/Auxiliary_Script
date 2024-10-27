#include "BigInt.h"

BigInt BigInt::big_zero(BigInt::_zero);
BigInt BigInt::big_one(BigInt::_one);
BigInt BigInt::big_two(BigInt::_two);

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

BigInt::BigInt(base_t &data, bool is_positive) {
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
        carry = ((*res->data)[i] - carry < unit);
    }

    for (size_t i = other->data->size(); i < (res->data->size())-1; i++) {
        unit = (*res->data)[i];
        (*res->data)[i] += carry;
        carry = ((*res->data)[i] - carry < unit);
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
            carry = (unit >> 64);
        }
        unit = (*res->data)[i+other->data->size()] + carry;
        #ifdef DEBUG_MODE_ON
            assert((unit >> 64) == 0);
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

    (*res->data)[res->data->size()-1] = ((*this->data)[res->data->size()-unit_shift-2] >> (BASE_WIDTH-part_shift));
    for (size_t i = res->data->size()-2; ; i--) {
        if (i > unit_shift) {
            (*res->data)[i] = ((*this->data)[i-unit_shift] << part_shift) | ((*this->data)[i-unit_shift-1] >> (BASE_WIDTH-part_shift));
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
            (*res->data)[i] = (((*this->data)[i+unit_shift]) >> part_shift) | (((*this->data)[i+unit_shift+1]) << (BASE_WIDTH-part_shift));
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

BigInt* BigInt::div(BigInt* other) {
    // if (this->compare(other) == LT) {
    //     base_t zero = 0;
    //     BigInt res = BigInt(zero);
    //     return &res;
    // }

    // vector<pair<BigInt*, BigInt*>> estimate;
    // BigInt cur_judge = BigInt(other);
    // base_t one = 1;
    // BigInt cur_level(one);

    // while (this->compare(other) == GT) {
        
    // }
    
}

BigInt* BigInt::fast_power(BigInt* exponet) {
    
}

#ifdef DEBUG_MODE_ON
void BigInt::print_plain(string auxiliary_text) {
    cout << auxiliary_text;
    for (size_t i = this->data->size() - 1; ; i-=1) {
        cout << hex << std::setw(16) << std::setfill('0') << (*this->data)[i] << dec;
        if (i == 0) {
            break;
        }
        // cout << ",";
    }
    cout << endl;
}
#endif

