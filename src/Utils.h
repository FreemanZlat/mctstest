#pragma once

#include <chrono>

class Utils
{
 public:
    class Timer
    {
     public:
        Timer();
        void start();
        int get();
     private:
        std::chrono::time_point<std::chrono::high_resolution_clock> start_point;
    };
};
