#pragma once

#include <chrono>
#include <cinttypes>

class Utils
{
 public:
    class Timer
    {
     public:
        Timer();
        void start();
        uint32_t get();
     private:
        std::chrono::time_point<std::chrono::high_resolution_clock> start_point;
    };
};
