#ifndef __TIMER__
#define __TIMER__

#include "Config.h"

class Timer{

public:
    void start();

    void stop();

    double elapsed();

    high_resolution_clock::time_point start_time;
    high_resolution_clock::time_point stop_time;
    bool running;

};

#endif