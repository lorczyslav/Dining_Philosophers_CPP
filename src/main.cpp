#include <cstring>
#include <functional>
#include <iostream>
#include <list>
#include <mutex>
#include <ncurses.h>
#include <string>
#include <thread>

#include "../include/philosopher.hpp"

namespace logger {

void log_info(std::string msg, std::mutex& mutex)
{
    auto lock = std::lock_guard(mutex);
    std::cout << msg << std::endl;
}

} // namespace logger

int main(int argc, char** argv)
{
    uint thread_count = 5;
    uint cycle_count = 10;

    for (int i = 1; i < argc; ++i) {
        try {
            if (!strcmp(argv[i], "-t") || !strcmp(argv[i], "--threads")) {
                thread_count = std::stoi(argv[i + 1]);
                ++i;
            } else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--count")) {
                cycle_count = std::stoi(argv[i + 1]);
                ++i;
            } else {
                std::cerr << "Unrecognised option \""
                          << argv[i]
                          << "\"."
                          << std::endl;
                return 1;
            }
        } catch (std::invalid_argument& e) {
            std::cerr << "Wrong format of an option's argument."
                      << std::endl;
            return 1;
        } catch (std::logic_error& e) {
            std::cerr << "Missing an argument for an option."
                      << std::endl;
            return 1;
        }
    }

    std::cout << "Threads: " << thread_count
              << ", Cycles: " << cycle_count
              << std::endl
              << std::endl;

    std::list<Philosopher> philos;
    std::list<std::thread> threads;

    for (uint i = 0; i < thread_count; ++i) {
        philos.emplace_back(cycle_count);
        threads.emplace_back(&Philosopher::run, &(philos.back()));
    }

    for (auto& t : threads)
        t.join();

    return 0;
}
