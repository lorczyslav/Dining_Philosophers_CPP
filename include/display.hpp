#pragma once
#include <algorithm>
#include <atomic>
#include <cstring>
#include <deque>
#include <iostream>
#include <map>
#include <ncurses.h>
#include <thread>

#include "../include/philosopher_state.hpp"

class Display {

    std::atomic<bool> _kill_switch;
    int _color_counter;
    unsigned _pcount;
    unsigned _fcount;
    unsigned _draw_start;

    std::map<unsigned, WINDOW*> _pwindows;
    std::map<unsigned, WINDOW*> _fwindows;
    std::deque<WINDOW*> _all_windows;

public:
    Display();
    ~Display();

    std::atomic<bool>& get_kill_switch();

    void init_sequence();
    void make_color_pairs();
    void destroy_window(WINDOW* win);
    void main_loop();

    // creates a window
    void create_title_frame();
    void create_philo_header();
    void create_fork_header();
    void create_pwin(unsigned pid, PhilosopherState state);
    void create_fwin(unsigned fid, unsigned user_id, bool dirty);

    // updates window contents
    void update_pwin(unsigned id, PhilosopherState state, int percentage);
    void update_fwin(unsigned id, unsigned user_id, bool dirty);

    const char* state_to_str(PhilosopherState state);
};