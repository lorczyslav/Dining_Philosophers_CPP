#pragma once
#include "../include/display.hpp"
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <tuple>

class Fork {

    Display& _controller;
    unsigned _user_id;

    std::condition_variable _cv;
    std::mutex _cv_mutex;
    std::mutex _fork_mutex;

    std::atomic<bool> _dirty;
    unsigned _id;

public:
    Fork(Display& display_controller, unsigned user_id);
    ~Fork() = default;

    void request(unsigned pid);
    void put_down();

    std::mutex& get_fork_mutex();
    std::tuple<unsigned, unsigned, bool> get_status();
};