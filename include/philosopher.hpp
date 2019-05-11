#pragma once
#include "../include/display.hpp"
#include "../include/fork.hpp"
#include "../include/philosopher_state.hpp"
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <tuple>

using sync_tuple = std::tuple<std::condition_variable&, std::mutex&, std::mutex&>;

class Philosopher {

    static std::mutex rng_mutex;

    Display& _controller;
    PhilosopherState _state;
    Fork& _left_fork;
    Fork& _right_fork;
    std::condition_variable& _cv;
    std::mutex& _table_mutex;
    std::mutex& _display_mutex;
    std::thread _plife_thread;
    std::atomic<bool>& _kill_switch;

    unsigned _id;

    void dine();
    void think();
    void run();

    void update_state(PhilosopherState state, int percentage);
    void update_forks_state();

    int get_random_duration(int from, int to);

public:
    Philosopher(Display& display_controller, Fork& left_fork, Fork& right_fork, sync_tuple st);
    ~Philosopher();
};
