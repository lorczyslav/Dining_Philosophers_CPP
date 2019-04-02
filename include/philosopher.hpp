#pragma once
#include "../include/fork.hpp"
#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <sstream>
#include <thread>

namespace logger {
extern void log_info(std::string, std::mutex&);
} // namespace logger

class Philosopher {

    static std::mutex philosopher_mutex;

    uint _cycle_count;
    uint _id;

    std::pair<Fork, Fork> _forks;

    void dine(uint cycles);
    void think(uint cycles);

public:
    Philosopher(uint cycle_count);
    ~Philosopher() = default;
    void run();
};