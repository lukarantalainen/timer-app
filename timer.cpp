#include <chrono>
#include <thread>
#include <iostream>
#include <libevdev-1.0/libevdev/libevdev.h>

#include "timer.h"

using namespace std::chrono_literals;


Timer::Timer() {
    start = std::chrono::steady_clock::now();

    while (true) {
        std::this_thread::sleep_for(1s);
        count();
        std::cout << "\r" << elapsed << "     "<< std::flush;
    }   
}

std::chrono::seconds Timer::Elapsed() {
    auto now = std::chrono::steady_clock::now();
    auto seconds {std::chrono::duration_cast<std::chrono::seconds>(now-start)};
    elapsed = seconds.count();
    return seconds;
}

void Timer::count() {
    ++elapsed;
}
