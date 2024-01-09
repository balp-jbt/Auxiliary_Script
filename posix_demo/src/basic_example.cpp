// g++ posix-demo.cpp -lpthread -o posix-demo.exe, remember to add -lpthread
// TODO: Only basic functions to build and join threads, not containing the lock or mutexs
#include <pthread.h>
#include <iostream>

using namespace std;

#define NUM_THREADS 3

// the params of pthread_create() are：id of pthread，attributes of pthreads as nullptr to be default,   
//   function of pthreads，params passed to pthreads  
#define TEST_FUNC(tid, attr, func, params)                                  \
    for(int i = 0; i < NUM_THREADS; i++) {                                  \
        int ret = pthread_create(tid, attr, func, params);                  \
        if (ret)                                                            \
            cout << "pthread_create error: error_code=" << ret << endl;     \
    }

// pthread_join's first param is the sub_thread to wait, second param is the ret value,
//   and ret value of itself is also the exec status, 0 as success.
#define JOIN_THREADS                                                    \
    for (int i = 0; i < NUM_THREADS; i++) {                             \
        int ret = pthread_join(tids[i], nullptr);                       \
        if (ret)                                                        \
            cout << "pthread_join error: error_code=" << ret << endl;   \
    }                                                                   


struct params_struct {
    int param1;
    double param2;
    string param3;    
};

// basic run and printf tid, tid is allocated automatically by operating system 
void* test_basic(void *arg) {
    cout << "current_id  " << pthread_self() << "\n";
    return nullptr; 
    // or pthread_exit(nullptr);
    // At least we need to have one to guarantee resources to bee released correctly 
}

// pass params to the function, need to transfer between (void*) and other types
void* test_param(void* param1) {
    double param = *((double *)param1); // firstly convert void* to double*, then gat its value
    cout << "current_id  " << pthread_self() << endl;
    cout << "param1  " << param << endl;
    return nullptr;
}

// get return value from the function
void* test_multi_param(void* param1) {
    params_struct params = *((params_struct*) param1);
    cout << "first param " << params.param1 << endl;
    cout << "second param " << params.param2 << endl;
    cout << "third param " << params.param3 << endl;
    return nullptr;
}

void begin_test(string name) {
    cout << "================== " << name << " ==================" << endl;
}

void end_test() {
    cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;
    cout << "\n\n\n";
}

int main() {
    pthread_t tids[NUM_THREADS];
    begin_test("test_basic");
    TEST_FUNC(&tids[i], NULL, test_basic, NULL);
    JOIN_THREADS;
    end_test();


    begin_test("test_param");
    double params[5] = {1.0, 0.2, 0.03, 0.004, 0.0005};
    TEST_FUNC(&tids[i], NULL, test_param, (void*)(&params[i%5]));
    JOIN_THREADS;
    end_test();



    begin_test("test_multi_param");
    int params_1[5] = {1, 2, 4, 8, 16};
    double params_2[5] = {1, 0.2, 0.03, 0.004, 0.0005};
    string params_3[5] = {"first str", "second str", "third str", "fourth str", "fifth str"};
    params_struct multi_params[5] = {};
    for (int i = 0; i < 5; i++) {
        params_struct new_multi_param = {params_1[i], params_2[i], params_3[i]};
        multi_params[i] = new_multi_param;
    }
    TEST_FUNC(&tids[i], NULL, test_multi_param, (void*)(&multi_params[i%5]));
    JOIN_THREADS;
    end_test();
}