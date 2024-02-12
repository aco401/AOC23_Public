#ifndef AOC_D19_THREAD_COORDINATOR
#define AOC_D19_THREAD_COORDINATOR

#include <condition_variable>
#include <cstdint>
#include <memory>
#include <mutex>
#include <shared_mutex>

namespace d19 {

class ThreadCoordinator {
  private:
    mutable std::shared_mutex start_mutex;
    std::unique_lock<std::shared_mutex> unique_lock;

    std::stop_source stop_source;

  public:
    ThreadCoordinator();
    ~ThreadCoordinator() = default; // --> Rule of 5

    // Copy
    ThreadCoordinator(const ThreadCoordinator &) = delete;
    ThreadCoordinator(ThreadCoordinator &&) = delete;

    // Move
    ThreadCoordinator &operator=(const ThreadCoordinator &) = delete;
    ThreadCoordinator &operator=(ThreadCoordinator &&) = delete;

    /**
     * @brief Wait with other threads to start some work at the same time
     * 
     */
    void waitToStart();

    /**
     * @brief Release all threads that are waiting for @a waitToStart()
     * 
     * @exception std::system_error if the function is called twice.
     */
    void releaseStart();

    /**
     * @brief Get a shared stop resource used to notify the completed work for threads  
     * 
     * @return std::stop_token 
     */
    [[nodiscard]] std::stop_token getStopResource();

    /**
     * @brief Notify "all" threads to stop working. Thereby, "all" means
     * every thread that listens on the stop_token from @a getStopResource. 
     * 
     */
    void requestStop();
};

} // namespace d19

#endif // AOC_D19_THREAD_COORDINATOR