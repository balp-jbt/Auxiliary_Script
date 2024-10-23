#include "BigInt.h"

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

BigInt::~BigInt() {
    delete this -> data;
}


BigInt* BigInt::add(BigInt *other, bool is_in_place) {
    
    #ifdef DEBUG_MODE_ON
        assert(this->is_positive && other->is_positive);
        assert(this->data->size() >= other->data->size());
    #endif

    BigInt* res;
    if (! is_in_place) {
        res = new BigInt();
        *(res->data) = *(this->data);
        res->data->resize(this->data->size()+1);
    } else {
        res = this;
        res->data->resize(this->data->size()+1);
    }
    
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

void BigInt::remove_leading_zero() {
    while ((*this->data)[this->data->size()-1] == 0 && this->data->size() > 0) {
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

