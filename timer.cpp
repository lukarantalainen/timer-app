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
    

    elapsed += loadTime();

    char output[50];


    std::thread t1(saveTime, this);

    while (true) {
        std::this_thread::sleep_for(1s);
        count();

        time(&date);
        struct tm datetime = *localtime(&date);
        char output[50];

        strftime(output, 50, "%D - %T", &datetime);

        std::cout << "\r" << elapsed << "     " << output << std::flush;
    }
}

double loadTime() {
    std::ifstream input(DATAPATH);
    std::string data;
    std::getline(input, data);
    if (data != "") {
        return stod(data);
    } else {
        return 0;
    }
    
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

double Timer::getElapsed() {
    return elapsed;
}

int main() {
    Timer timer;

    return 0;

}
