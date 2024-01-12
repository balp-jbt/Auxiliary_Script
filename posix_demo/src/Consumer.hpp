#include <iostream>

#include "Consumer.h"

template <class T>
Consumer<T>::Consumer(Buffer<T>* buffer, pthread_mutex_t* buffer_lock, 
    pthread_cond_t* buffer_produce_signal, pthread_cond_t* buffer_consume_signal, int* exit_cond):
        buffer(buffer), buffer_lock(buffer_lock),
            buffer_produce_signal(buffer_produce_signal), 
                buffer_consume_signal(buffer_consume_signal), exit_cond(exit_cond) {}

template <class T>
void* Consumer<T>::consumer_thread_entrance(void* args) {
    Consumer<T>* consumer = static_cast<Consumer<T>*>(args);
    consumer->consumer_thread();
    std::cout << "----------Consumer's END" << std::endl;
    pthread_exit(nullptr);
}

template <class T>
void Consumer<T>::consumer_thread() {
    while(true) {
        pthread_mutex_lock(buffer_lock);
        std::cout << "----------Consumer's Main Loop" << std::endl;
        while(buffer->is_empty() && !(*exit_cond)) {
            pthread_cond_wait(buffer_produce_signal, buffer_lock);
        }
        T* source = buffer->get_source();
        pthread_mutex_unlock(buffer_lock);
        if (*exit_cond && !source) {
            return;
        }
        consume(source);
        pthread_cond_signal(buffer_consume_signal);
    }
}

template <class T>
void Consumer<T>::consume(T* source) {
    pthread_mutex_lock(buffer_lock);
    std::cout << "----------Consumer is consuming!" << std::endl;
    pthread_mutex_unlock(buffer_lock);

    int res = source->use();

    pthread_mutex_lock(buffer_lock);
    std::cout << "----------Source val {" << res << "} got used" << std::endl;
    pthread_mutex_unlock(buffer_lock);
}
