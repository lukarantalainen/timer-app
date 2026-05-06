#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
    public:
    Timer();

    std::chrono::seconds Elapsed();


    private:
    std::chrono::time_point<std::chrono::steady_clock> start{};
    double elapsed{};

    void count();
};


#endif
