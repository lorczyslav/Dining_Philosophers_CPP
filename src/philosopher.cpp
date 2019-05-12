#include "../include/philosopher.hpp"

// init static resources
std::mutex Philosopher::rng_mutex;

Philosopher::Philosopher(Display& display_controller, Fork& left_fork, Fork& right_fork, sync_tuple st)
    : _controller(display_controller)
    , _state(PhilosopherState::READY)
    , _left_fork(left_fork)
    , _right_fork(right_fork)
    , _cv(std::get<0>(st))
    , _table_mutex(std::get<1>(st))
    , _display_mutex(std::get<2>(st))
    , _plife_thread(&Philosopher::run, this)
    , _kill_switch(_controller.get_kill_switch())
{
    static unsigned ids = 1;
    _id = ids++;
    _controller.create_pwin(_id, _state);
}

Philosopher::~Philosopher()
{
    if (_plife_thread.joinable())
        _plife_thread.join();
}

void Philosopher::run()
{
    std::unique_lock lock{ _table_mutex };
    _cv.wait(lock);
    lock.unlock();
    while (!_kill_switch) {
        think();
        dine();
    }
}

void Philosopher::dine()
{
    update_state(PhilosopherState::WAITING, -1);
    _left_fork.request(_id);
    _right_fork.request(_id);
    update_forks_state();
    std::scoped_lock lock(_left_fork.get_fork_mutex(), _right_fork.get_fork_mutex());

    int dur = get_random_duration(25, 45);
    for (int i = 0; i < 100; ++i) {
        update_state(PhilosopherState::EATING, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(dur));
    }

    _left_fork.put_down();
    _right_fork.put_down();
    update_forks_state();
}

void Philosopher::think()
{
    int dur = get_random_duration(25, 45);
    for (int i = 0; i < 100; ++i) {
        update_state(PhilosopherState::THINKING, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(dur));
    }
}

void Philosopher::update_state(PhilosopherState state, int percentage)
{
    if (!_kill_switch) {
        std::lock_guard lg{ _display_mutex };
        _controller.update_pwin(_id, state, percentage);
        _state = state;
    }
}

void Philosopher::update_forks_state()
{
    if (!_kill_switch) {
        std::lock_guard lg{ _display_mutex };
        auto tp = _left_fork.get_status();
        _controller.update_fwin(std::get<0>(tp), std::get<1>(tp), std::get<2>(tp));
        tp = _right_fork.get_status();
        _controller.update_fwin(std::get<0>(tp), std::get<1>(tp), std::get<2>(tp));
    }
}

int Philosopher::get_random_duration(int from, int to)
{
    static thread_local std::mt19937 gen;
    std::uniform_int_distribution<> dist(from, to);
    return dist(gen);
}
