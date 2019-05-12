#include "../include/display.hpp"

Display::Display()
    : _kill_switch(false)
    , _color_counter(9)
    , _pcount(0)
    , _fcount(0)
    , _draw_start(6)
{
    init_sequence();
    make_color_pairs();
    create_title_frame();
    create_philo_header();
    create_fork_header();
}

Display::~Display()
{
}

std::atomic<bool>& Display::get_kill_switch()
{
    return _kill_switch;
}

void Display::init_sequence()
{
    initscr();
    raw();
    noecho();
    curs_set(false);
    start_color();
    use_default_colors();
    keypad(stdscr, true);
    refresh();
}

void Display::destroy_window(WINDOW* win)
{
    wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(win);
    delwin(win);
}

void Display::make_color_pairs()
{
    for (int i = 0; i < 256; ++i) {
        init_pair(i + 1, i, -1);
    }
}

void Display::create_title_frame()
{
    WINDOW* title_window = ::newwin(3, ::COLS, 0, 0);
    _all_windows.push_back(title_window);
    wattron(title_window, COLOR_PAIR(4));
    box(title_window, 0, 0);
    wattroff(title_window, COLOR_PAIR(4));

    wattron(title_window, COLOR_PAIR(6));
    mvwprintw(title_window, 1, ::COLS / 2 - 25, "Dining Philosophers using Chandy Misra Algorithm");
    wattroff(title_window, COLOR_PAIR(6));
    wrefresh(title_window);
}

void Display::create_philo_header()
{
    WINDOW* pheader = ::newwin(4, ::COLS / 2, 3, 0);
    _all_windows.push_back(pheader);
    mvwhline(pheader, 0, 0, ACS_HLINE, ::COLS / 2);
    mvwhline(pheader, 2, 0, ACS_HLINE, ::COLS / 2);
    mvwvline(pheader, 1, 0, ACS_VLINE, 1);
    mvwvline(pheader, 1, ::COLS / 2 - 1, ACS_VLINE, 1);

    wattron(pheader, COLOR_PAIR(0));
    mvwprintw(pheader, 1, 1, "Philosopher");
    mvwprintw(pheader, 1, 15, "Status");
    mvwprintw(pheader, 1, 35, "Progress");
    wattroff(pheader, COLOR_PAIR(0));
    wrefresh(pheader);
}

void Display::create_fork_header()
{
    WINDOW* fheader = ::newwin(4, ::COLS / 2, 3, ::COLS / 2);
    _all_windows.push_back(fheader);
    mvwhline(fheader, 0, 0, ACS_HLINE, ::COLS / 2);
    mvwhline(fheader, 2, 0, ACS_HLINE, ::COLS / 2);
    mvwvline(fheader, 1, 0, ACS_VLINE, 1);
    mvwvline(fheader, 1, ::COLS / 2 - 1, ACS_VLINE, 1);

    wattron(fheader, COLOR_PAIR(0));
    mvwprintw(fheader, 1, 1, "Fork");
    mvwprintw(fheader, 1, 15, "Status");
    mvwprintw(fheader, 1, 35, "Last user");
    wattroff(fheader, COLOR_PAIR(0));
    wrefresh(fheader);
}

void Display::create_pwin(unsigned pid, PhilosopherState state)
{
    WINDOW* nhw = ::newwin(2, ::COLS / 2, _draw_start + (2 * _pcount++), 0);
    _all_windows.push_back(nhw);
    _pwindows[pid] = nhw;
    mvwhline(nhw, 1, 0, ACS_HLINE, ::COLS / 2);
    mvwvline(nhw, 0, 0, ACS_VLINE, 1);
    mvwvline(nhw, 0, ::COLS / 2 - 1, ACS_VLINE, 1);
    update_pwin(pid, state, -1);
    wrefresh(nhw);
}

void Display::create_fwin(unsigned fid, unsigned user_id, bool dirty)
{
    WINDOW* nhw = ::newwin(2, ::COLS / 2, _draw_start + (2 * _fcount++), ::COLS / 2);
    _fwindows[fid] = nhw;
    _all_windows.push_back(nhw);
    mvwhline(nhw, 1, 0, ACS_HLINE, ::COLS / 2);
    mvwvline(nhw, 0, 0, ACS_VLINE, 1);
    mvwvline(nhw, 0, ::COLS / 2 - 1, ACS_VLINE, 1);
    update_fwin(fid, user_id, dirty);
    wrefresh(nhw);
}

void Display::update_pwin(unsigned id, PhilosopherState state, int percentage)
{
    WINDOW* win = _pwindows[id];
    wattron(win, COLOR_PAIR(_color_counter + id * 5));
    mvwprintw(win, 0, 1, "%d", id);
    mvwprintw(win, 0, 15, "%s", state_to_str(state));
    if (percentage >= 0) {
        mvwprintw(win, 0, 30, "%02d%%", percentage);
        for (int i = 0; i < 20; ++i) {
            if (percentage / 5 > i)
                mvwprintw(win, 0, 35 + i, "|");
            else
                mvwprintw(win, 0, 35 + i, ".");
        }
    } else {
        mvwprintw(win, 0, 30, "N/A");
        mvwprintw(win, 0, 35, "--------------------");
    }

    wattroff(win, COLOR_PAIR(_color_counter + id));
    wrefresh(win);
}

void Display::update_fwin(unsigned id, unsigned user_id, bool dirty)
{
    WINDOW* win = _fwindows[id];
    wattron(win, COLOR_PAIR(_color_counter + id * 3));
    mvwprintw(win, 0, 1, "%02d", id);
    mvwprintw(win, 0, 15, "%s", dirty ? "Dirty" : "Clean");
    mvwprintw(win, 0, 35, "%02d", user_id);
    wattroff(win, COLOR_PAIR(_color_counter + id));
    wrefresh(win);
}

const char* Display::state_to_str(PhilosopherState state)
{
    if (state == PhilosopherState::READY) {
        return "READY   ";
    } else if (state == PhilosopherState::EATING) {
        return "EATING  ";
    } else if (state == PhilosopherState::THINKING) {
        return "THINKING";
    } else if (state == PhilosopherState::WAITING) {
        return "WAITING ";
    } else if (state == PhilosopherState::DEAD) {
        return "DEAD    ";
    } else
        return "UNKNOWN ";
}

void Display::main_loop()
{
    while (wgetch(stdscr) != 27) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    _kill_switch = true;
    std::for_each(_all_windows.begin(), _all_windows.end(), [this](auto a) { destroy_window(a); });
    endwin();
}
