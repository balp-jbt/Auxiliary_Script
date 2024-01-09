// By ChatGPT
#include <iostream>
#include <queue>
#include <pthread.h>

const int BUFFER_SIZE = 5;

std::queue<int> buffer;
pthread_mutex_t mtx;
pthread_cond_t bufferEmpty, bufferFull;

void* producer(void* arg) {
    for (int i = 0; i < 10; ++i) {
        pthread_mutex_lock(&mtx);

        while (buffer.size() == BUFFER_SIZE) {
            pthread_cond_wait(&bufferFull, &mtx);
        }

        int item = i;
        buffer.push(item);
        std::cout << "Produced: " << item << std::endl;

        pthread_mutex_unlock(&mtx);
        pthread_cond_signal(&bufferEmpty);
    }

    pthread_exit(NULL);
}

void* consumer(void* arg) {
    for (int i = 0; i < 10; ++i) {
        pthread_mutex_lock(&mtx);

        while (buffer.empty()) {
            pthread_cond_wait(&bufferEmpty, &mtx);
        }

        int item = buffer.front();
        buffer.pop();
        std::cout << "Consumed: " << item << std::endl;

        pthread_mutex_unlock(&mtx);
        pthread_cond_signal(&bufferFull);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t producerThread, consumerThread;

    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&bufferEmpty, NULL);
    pthread_cond_init(&bufferFull, NULL);

    pthread_create(&producerThread, NULL, producer, NULL);
    pthread_create(&consumerThread, NULL, consumer, NULL);

    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);

    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&bufferEmpty);
    pthread_cond_destroy(&bufferFull);

    return 0;
}
