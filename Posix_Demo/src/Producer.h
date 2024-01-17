#ifndef __PRODUCER__
#define __PRODUCER__
#include <pthread.h>

#include "Buffer.h"

template <class T>
class Producer {

private:
    Buffer<T>* buffer;
    pthread_mutex_t* buffer_lock;
    pthread_cond_t* buffer_produce_signal;
    pthread_cond_t* buffer_consume_signal;
    int* exit_cond;

public:
    Producer(Buffer<T>* buffer, pthread_mutex_t* buffer_lock, 
        pthread_cond_t* buffer_produce_signal, pthread_cond_t* buffer_consume_signal, 
        int *exit_cond);

    static void* producer_thread_entrance(void* args);

    void producer_thread();

    T* produce();

};
#endif