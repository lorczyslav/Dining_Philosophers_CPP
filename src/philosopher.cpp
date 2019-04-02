#include "../include/philosopher.hpp"

std::mutex Philosopher::philosopher_mutex;

Philosopher::Philosopher(uint cycle_count)
    : _cycle_count(cycle_count)
    , _forks(Fork(), Fork())
{
    static uint ids = 1;
    _id = ids++;
}

void Philosopher::run()
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<> bool_dist(0, 1);
    std::uniform_int_distribution<> sleep_dist(2500, 3500);

    uint cycles = 0;
    while (cycles++ < _cycle_count) {
        if (bool_dist(rng)) {
            _forks.first.use();
            _forks.second.use();
            dine(cycles);
            _forks.first.leave();
            _forks.second.leave();
        } else {
            think(cycles);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_dist(rng)));
    }
}

void Philosopher::think(uint cycles)
{
    std::ostringstream oss;
    oss << "Philosopher " << _id << "\tis thinking *Hmmmmm.....* "
        << "(" << cycles << "/" << _cycle_count << ")";
    logger::log_info(oss.str(), philosopher_mutex);
}

void Philosopher::dine(uint cycles)
{

    std::ostringstream oss;
    oss << "Philosopher " << _id << "\tis eating.  *Chomp Chomp* "
        << "(" << cycles << "/" << _cycle_count << ")";
    logger::log_info(oss.str(), philosopher_mutex);
}
