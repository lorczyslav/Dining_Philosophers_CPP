#pragma once
#include "../include/display.hpp"
#include "../include/fork.hpp"
#include "../include/philosopher.hpp"
#include <atomic>
#include <deque>

using sync_tuple = std::tuple<std::condition_variable&, std::mutex&, std::mutex&>;

class TableSetup {

    static std::condition_variable cv;
    static std::mutex table_mutex;
    static std::mutex display_mutex;

    unsigned _pcount;
    std::deque<Philosopher> _philosophers;
    std::deque<Fork> _forks;
    Display& _display_ct;

    void make_forks();
    void spawn_philosophers();
    void set_table();
    sync_tuple get_sync_tools();

public:
    TableSetup(unsigned pcount, Display& display_controller);
};