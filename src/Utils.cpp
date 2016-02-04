#include "Utils.h"

Utils::Timer::Timer()
{
    this->start();
}

void Utils::Timer::start()
{
    this->start_point = std::chrono::high_resolution_clock::now();
}

int Utils::Timer::get()
{
    auto current = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(current - this->start_point).count();
}
