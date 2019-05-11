#include <cstring>
#include <functional>
#include <iostream>
#include <list>
#include <mutex>
#include <ncurses.h>
#include <string>
#include <thread>

#include "../include/display.hpp"
#include "../include/philosopher.hpp"
#include "../include/table_setup.hpp"

namespace parser {

unsigned parse_args(int argc, char** argv)
{
    unsigned thread_count = 5;

    for (int i = 1; i < argc; ++i) {
        try {
            if (!strcmp(argv[i], "-t") || !strcmp(argv[i], "--threads")) {
                thread_count = std::stoi(argv[i + 1]);
                if (thread_count > 100) {
                    std::cerr << "Number of threads cannot be greater than 100."
                              << std::endl;
                    std::exit(1);
                }
                ++i;
            } else {
                std::cerr << "Unrecognised option \""
                          << argv[i]
                          << "\"."
                          << std::endl;
                std::exit(1);
            }
        } catch (std::invalid_argument& e) {
            std::cerr << "Wrong format of an option's argument."
                      << std::endl;
            std::exit(1);
        } catch (std::logic_error& e) {
            std::cerr << "Missing an argument for an option."
                      << std::endl;
            std::exit(1);
        }
    }
    return thread_count;
}

} // namespace parser

int main(int argc, char** argv)
{
    auto philosopher_count = parser::parse_args(argc, argv);
    Display display_controller{};
    TableSetup table{ philosopher_count, display_controller };

    return 0;
}
