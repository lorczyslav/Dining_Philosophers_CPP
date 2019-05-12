#include "../include/table_setup.hpp"

// init static resources
std::condition_variable TableSetup::cv;
std::mutex TableSetup::table_mutex;
std::mutex TableSetup::display_mutex;

TableSetup::TableSetup(unsigned pcount, Display& display_controller)
    : _pcount(pcount)
    , _display_ct(display_controller)
{
    set_table();
}

void TableSetup::make_forks()
{
    for (unsigned i = 1; i < _pcount; ++i) {
        _forks.emplace_back(_display_ct, i);
    }
    _forks.emplace_back(_display_ct, 1);
}

void TableSetup::spawn_philosophers()
{
    _philosophers.emplace_back(_display_ct, _forks.back(), _forks.front(), get_sync_tools());
    for (unsigned i = 1; i < _pcount; ++i) {
        _philosophers.emplace_back(_display_ct, _forks.at(i - 1), _forks.at(i), get_sync_tools());
    }
}

void TableSetup::set_table()
{
    make_forks();
    spawn_philosophers();
    // wait for all threads to initialize
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    cv.notify_all();
    _display_ct.main_loop();
}

sync_tuple TableSetup::get_sync_tools()
{
    return std::forward_as_tuple(cv, table_mutex, display_mutex);
}