// g++ posix-demo.cpp -lpthread -o posix-demo.exe
#include <pthread.h>
#include <iostream>

using namespace std;

#define NUM_THREADS 1

void* sub_func(void *param1) {
    unsigned tid = *((pthread_t *)param1);
    cout << "current_id  " << pthread_self() << "\n";
    cout << "param1  " << tid << endl;
    return (void*)1;
}

int main() {
    pthread_t tids[NUM_THREADS];
    for(pthread_t i = 0; i < NUM_THREADS; i++) {
        tids[i] = i;
        cout << "Main -> tid=" << tids[i] << endl;
        //the params are：id of pthread，params of pthreads，function of pthreads，params passed to pthreads
        int ret = pthread_create(&tids[i], NULL, sub_func, (void*)&(tids[i])); // Here must use '&' to get address
        if (ret != 0)
        {
            cout << "pthread_create error: error_code=" << ret << endl;
        }
    }
    pthread_exit(NULL);
}