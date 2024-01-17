#include <stdexcept>

#include "Buffer.h"

template <class T>
Buffer<T>::Buffer() {}

template <class T>
T* Buffer<T>::get_source() {
    if (buffer.empty()) {
        return NULL;
    }
    T* res = buffer.front();
    buffer.pop();
    return res;
}

template <class T>
bool Buffer<T>::is_full() {
    if (buffer.size() > MAX_BUFFER_SIZE) {
        throw std::runtime_error("^^^^^^^^^^^^^^^^ Err in Buffer size ^^^^^^^^^^^^^^^^");
    }
    return buffer.size() == MAX_BUFFER_SIZE;
}

template <class T>
bool Buffer<T>::is_empty() {
    return buffer.empty();
}

template <class T>
void Buffer<T>::add_source(T* new_source) {
    if (buffer.size() >= MAX_BUFFER_SIZE) {
        throw std::runtime_error("^^^^^^^^^^^^^^^^ Err in Buffer size ^^^^^^^^^^^^^^^^");
    }
    buffer.push(new_source);
}
