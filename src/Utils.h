#pragma once

#include <chrono>
#include <cinttypes>
#include <random>

class Timer
{
 public:
    Timer();
    void start();
    uint32_t get();
 private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_point;
};

class Random
{
 public:
    Random();
    uint32_t get();
 private:
    std::mt19937 mt;
    std::uniform_int_distribution<uint32_t> uint_dist;
};
