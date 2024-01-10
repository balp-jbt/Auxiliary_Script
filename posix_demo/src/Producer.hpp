#include <iostream>

#include "Producer.h"

template <class T>
Producer<T>::Producer(Buffer<T>* buffer, pthread_mutex_t* buffer_lock, 
    pthread_cond_t* buffer_produce_signal, pthread_cond_t* buffer_consume_signal):
        buffer(buffer), buffer_lock(buffer_lock),
            buffer_produce_signal(buffer_produce_signal), 
                buffer_consume_signal(buffer_consume_signal) {}

template <class T>
void* Producer<T>::producer_thread_entrance(void* args) {
    Producer<T>* producer = static_cast<Producer<T>*>(args);
    producer->producer_thread();
    pthread_exit(nullptr);
}

template <class T>
void Producer<T>::producer_thread() {
    while(true) {
        pthread_mutex_lock(buffer_lock);
        while (buffer->is_full()) {
            pthread_cond_wait(buffer_consume_signal, buffer_lock);
        }
        pthread_mutex_unlock(buffer_lock);

        T* new_source = produce();

        pthread_mutex_lock(buffer_lock);
        buffer->add_source(new_source);
        pthread_mutex_unlock(buffer_lock);

        pthread_cond_signal(buffer_produce_signal);
    }
}

template <class T>
T* Producer<T>::produce() {
    pthread_mutex_lock(buffer_lock);
    std::cout << "Producer is working!" << std::endl;
    std::cout << "Input val to generate a source:" << std::endl;
    int val;
    pthread_mutex_unlock(buffer_lock);
    std::cin >> val;
    
    T* new_source = T::generate(val);
    return new_source;
}
