#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
    public:
    Timer();
    double getElapsed();

    private:
    std::chrono::time_point<std::chrono::steady_clock> start{};
    time_t date;

    double elapsed{};

    void count();
    friend void saveTime(Timer* timer);

    
};

double loadTime();
void saveTime(Timer* timer);


#endif
