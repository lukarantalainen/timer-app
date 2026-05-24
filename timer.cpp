#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

#include "timer.h"

using namespace std::chrono_literals;

#define DATAPATH "data.txt"

Timer::Timer() {
    start = std::chrono::steady_clock::now();
    time(&date);
    struct tm datetime = *localtime(&date);

    elapsed += loadTime();

    char output[50];

    strftime(output, 50, "%a %D %H", &datetime);
    std::cout << output;

    std::thread t1(saveTime, this);

    while (true) {
        std::this_thread::sleep_for(1s);
        count();
        std::cout << "\r" << elapsed << "     "<< std::flush;
    }
}

double loadTime() {
    std::ifstream input(DATAPATH);
    std::string data;
    std::getline(input, data);
    return stod(data);
}

void saveTime(Timer* timer) {
    while (true) {
        std::this_thread::sleep_for(60s);
        std::ofstream output(DATAPATH);
        output << timer->elapsed;
    }
}

void Timer::count() {
    ++elapsed;
}
