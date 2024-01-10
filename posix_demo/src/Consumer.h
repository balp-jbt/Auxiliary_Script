#ifndef __CONSUMER__
#define __CONSUMER__
#include <pthread.h>

#include "Buffer.h"

template <class T>
class Consumer {

private:
    Buffer<T>* buffer;
    pthread_mutex_t* buffer_lock;
    pthread_cond_t* buffer_produce_signal;
    pthread_cond_t* buffer_consume_signal;
public:
    Consumer(Buffer<T>* buffer, pthread_mutex_t* buffer_lock, 
        pthread_cond_t* buffer_produce_signal, pthread_cond_t* buffer_consume_signal);
    static void* consumer_thread_entrance(void* args);

    void consumer_thread();

    void consume(T* source);


};
#endif