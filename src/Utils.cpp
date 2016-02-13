#include "Utils.h"

Timer::Timer()
{
    this->start();
}

void Timer::start()
{
    this->start_point = std::chrono::high_resolution_clock::now();
}

uint32_t Timer::get()
{
    auto current = std::chrono::high_resolution_clock::now();
    return (uint32_t) std::chrono::duration_cast<std::chrono::milliseconds>(current - this->start_point).count();
}

Random::Random()
{
    std::random_device rd;
    this->mt.seed(rd());
}

uint32_t Random::get()
{
    return this->uint_dist(mt);
}
