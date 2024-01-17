#include <iostream>

#include "Producer.h"

template <class T>
Producer<T>::Producer(Buffer<T>* buffer, pthread_mutex_t* buffer_lock, 
    pthread_cond_t* buffer_produce_signal, pthread_cond_t* buffer_consume_signal, int* exit_cond):
        buffer(buffer), buffer_lock(buffer_lock),
            buffer_produce_signal(buffer_produce_signal), 
                buffer_consume_signal(buffer_consume_signal), exit_cond(exit_cond) {}

template <class T>
void* Producer<T>::producer_thread_entrance(void* args) {
    Producer<T>* producer = static_cast<Producer<T>*>(args);
    producer->producer_thread();
    std::cout << "----------Producer's END" << std::endl;
    pthread_exit(nullptr);
}

template <class T>
void Producer<T>::producer_thread() {
    while(true) {
        pthread_mutex_lock(buffer_lock);
        std::cout << "----------Producer's Main Loop" << std::endl;
        while (buffer->is_full()) {
            pthread_cond_wait(buffer_consume_signal, buffer_lock);
        }
        pthread_mutex_unlock(buffer_lock);
        T* new_source = produce();

        pthread_mutex_lock(buffer_lock);
        buffer->add_source(new_source);
        pthread_mutex_unlock(buffer_lock);
        pthread_cond_signal(buffer_produce_signal);
        if (! new_source) {
            return;
        }
    }
}

template <class T>
T* Producer<T>::produce() {
    pthread_mutex_lock(buffer_lock);
    std::cout << "----------Producer is producing!" << std::endl;
    std::cout << "----------Input val to generate a source, -1 to end:" << std::endl;
    int val;
    pthread_mutex_unlock(buffer_lock);
    std::cin >> val;
    if (val == -1) {
        *exit_cond = 1;
        return nullptr;    
    }
    T* new_source = T::generate(val);
    return new_source;
}
