#include <time.h>
#include <stdexcept>

#include "Source.h"

int Source::id = 0;

Source::Source(int val) {
    this->val = val;
    Source::id++;
}

void Source::reset_id() {
    id = 0;
}

Source* Source::generate(int val) {
    Source* res = new Source(val);
    return res;
}

int Source::use() {
    struct timespec sleep_time, remaining_time;
    sleep_time.tv_sec = 5;
    sleep_time.tv_nsec = 0;
    if (nanosleep(&sleep_time, &remaining_time) != 0) {
        throw std::runtime_error("^^^^^^^^^^^^^^^^ Err ^^^^^^^^^^^^^^^^");
    }
    int res = val;
    delete this;
    return res;
}
