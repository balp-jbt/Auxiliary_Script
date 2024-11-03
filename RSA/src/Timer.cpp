#include "Timer.h"

void Timer::start() {
    start_time = std::chrono::high_resolution_clock::now();
    running = true;
}

void Timer::stop() {
    if (running) {
        stop_time = std::chrono::high_resolution_clock::now();
        running = false;
    }
}

double Timer::elapsed() {
    if (running) {
        auto current_time = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(current_time - start_time).count();
    } else {
        return std::chrono::duration<double>(stop_time - start_time).count();
    }
}