
#include <cassert>
#include <chrono>

#include <ElapsedTimer.hpp>

d19::ElapsedTimer::ElapsedTimer()
    : point_start{std::chrono::duration<unsigned, std::milli>(0)},
      point_stop{std::chrono::duration<unsigned, std::milli>(0)}, start_cnt{0},
      stop_cnt{0} {};

void d19::ElapsedTimer::start() {
    // To check correct usage
    if (stop_cnt == 0) {
        ++start_cnt;
        point_start = std::chrono::steady_clock::now();
    }
};

void d19::ElapsedTimer::stop() {
    if (start_cnt == 1) {
        ++stop_cnt;
        point_stop = std::chrono::steady_clock::now();
    }
};

void d19::ElapsedTimer::clear() {
    start_cnt = 0;
    stop_cnt = 0;
};

std::chrono::milliseconds d19::ElapsedTimer::getElapsedTime() {
    if (stop_cnt != start_cnt || start_cnt != 1 || stop_cnt != 1) {
        throw std::domain_error(
            "Wrong usage of functions ElapsedTimer::start(), "
            "ElapsedTimer::stop() and "
            "ElapsedTimer::clear().");
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(point_stop -
                                                                 point_start);
};
