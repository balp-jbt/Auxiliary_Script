#include <pthread.h>

#include "Producer.hpp"
#include "Consumer.hpp"
#include "Buffer.hpp"
#include "Source.hpp"

#define INIT_PARAMS &buffer, &buffer_lock, \
            &buffer_produce_signal, &buffer_consume_signal

int main() {
    pthread_t producer_tid, consumer_tid;
    pthread_mutex_t buffer_lock;
    pthread_cond_t buffer_produce_signal, buffer_consume_signal;

    Buffer<Source> buffer = Buffer<Source>();
    Producer<Source> producer = Producer<Source>(INIT_PARAMS);

    Consumer<Source> consumer = Consumer<Source>(INIT_PARAMS);

    pthread_create(&producer_tid, NULL, Producer<Source>::producer_thread_entrance, (void*)&producer);

    pthread_create(&consumer_tid, NULL, Consumer<Source>::consumer_thread_entrance, (void*)&consumer);

    pthread_join(producer_tid, NULL);
    pthread_join(consumer_tid, NULL);

    return 0;
}
