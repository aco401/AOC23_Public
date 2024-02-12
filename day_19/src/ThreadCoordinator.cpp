#include <stop_token>

#include <ThreadCoordinator.hpp>

#include <shared_mutex>

d19::ThreadCoordinator::ThreadCoordinator() : unique_lock{start_mutex} {}

void d19::ThreadCoordinator::waitToStart() {
    std::shared_lock lock(start_mutex);
}

void d19::ThreadCoordinator::releaseStart() { unique_lock.unlock(); }

std::stop_token d19::ThreadCoordinator::getStopResource() {
    return stop_source.get_token();
};

void d19::ThreadCoordinator::requestStop() { stop_source.request_stop(); };
