#include "../include/fork.hpp"

Fork::Fork(Display& display_controller, unsigned user_id)
    : _controller(display_controller)
    , _user_id(user_id)
    , _dirty(true)
{
    static unsigned ids = 1;
    _id = ids++;
    _controller.create_fwin(_id, _user_id, _dirty);
}

void Fork::request(unsigned pid)
{
    std::unique_lock lock{ _cv_mutex };
    _cv.wait(lock, [&]() -> bool { return _dirty; });
    std::lock_guard lg(_fork_mutex);
    _dirty = false;
    _user_id = pid;
}

void Fork::put_down()
{
    std::unique_lock ul{ _cv_mutex };
    _dirty = true;
    _cv.notify_all();
}

std::mutex& Fork::get_fork_mutex()
{
    return _fork_mutex;
}

std::tuple<unsigned, unsigned, bool> Fork::get_status()
{
    return { _id, _user_id, _dirty };
}