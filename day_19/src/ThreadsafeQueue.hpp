#ifndef AOC_D19_ATOMIC_QUEUE
#define AOC_D19_ATOMIC_QUEUE

#include <TraceException.hpp>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>
#include <queue>
#include <utility>

#include <Workflow.hpp>

namespace d19 {

/**
 * @brief A condition_variable wrapper that calls notify 
 * on all waiting threads before destruction and unblocks execution.
 * 
 */
class condition_variable_last_notify {
    std::condition_variable cv;
    std::atomic_bool destruction{false};

  public:
    condition_variable_last_notify() = default;
    ~condition_variable_last_notify() {
        destruction = true;
        notify_all();
    };

    // Copying deleted
    condition_variable_last_notify(const condition_variable_last_notify &) =
        delete;
    condition_variable_last_notify &
    operator=(const condition_variable_last_notify &) = delete;

    // Moving deleted
    condition_variable_last_notify(condition_variable_last_notify &&) = delete;
    condition_variable_last_notify &
    operator=(condition_variable_last_notify &&) = delete;

    void notify_one() noexcept { cv.notify_one(); }

    void notify_all() noexcept { cv.notify_all(); }

    template <typename _Rep, typename _Period>
    bool wait_for(std::unique_lock<std::mutex> &__lock,
                  const std::chrono::duration<_Rep, _Period> &__rtime,
                  std::function<bool()> __f) {
        return cv.wait_for(__lock, __rtime,
                           [this, __f] { return __f() || destruction; });
    }

    void wait(std::unique_lock<std::mutex> &__lock, std::function<bool()> __f) {

        cv.wait(__lock, [this, __f] { return __f() || destruction; });
    };
};

/**
 * @brief A threadsafe queue wrapper. 
 * 
 * @tparam T Type of elements to store in the queue.
 */
template <typename T> class ThreadsafeQueue {
  private:
    std::queue<T> queue;
    std::mutex mutex;
    condition_variable_last_notify cv;

  public:
    ThreadsafeQueue() = default;
    ~ThreadsafeQueue() = default;

    // Copying deleted
    ThreadsafeQueue(const ThreadsafeQueue &) = delete;
    ThreadsafeQueue &operator=(const ThreadsafeQueue &) = delete;

    // Moving deleted
    ThreadsafeQueue(ThreadsafeQueue &&) = delete;
    ThreadsafeQueue &operator=(ThreadsafeQueue &&) = delete;

    /**
     * @brief Construct a new ThreadsafeQueue object from a queue.
     * Should be used in combination with @a std::queue<T> @a data();
     * 
     */
    ThreadsafeQueue(std::queue<T> &&);

    /**
     * @brief Get a moved copy of the internal queue data.
     * Used in conjunction with Constructor @a ThreadsafeQueue(const std::queue<T> &other).
     * 
     * @tparam T template type
     * @return const std::queue<T>& internal queue data
     * 
    */
    std::queue<T> data();

    /**
     * @brief Returns the amount of elements in the queue
     * 
     * @return size_t amount elements
     */
    [[nodiscard]] size_t size();

    /**
     * @brief Are there no elements in the queue?
     * 
     * @return true 
     * @return false 
     */
    [[nodiscard]] bool empty();

    /**
     * @brief Returns and removes the first element from the queue
     * 
     * @return T removed element 
     * 
     * @exception std::length_error if the queue is empty when the function is called
     */
    [[nodiscard]] T pop();

    /**
     * @brief Insert a given element of @a elem to the end of the queue
     * 
     * @param elem element to insert
     * 
     * @warning unblocks if the ThreadsafeQueue is deleted
     */
    void push(T elem);

    /**
     * @brief Waits as long as the queue minimally stores an element in the queue
     * 
     * @warning blocks the threads - forever if no element is pushed from another thread
     */
    void waitOnNotEmpty();

    /**
     * @brief Waits as long as the queue minimally stores an element in the queue. 
     * Or the given time in param @a timeout has passed.
     * 
     * @param timeout maximum time the thread is blocked
     * 
     * @warning unblocks if the ThreadsafeQueue is deleted
     */
    void waitOnNotEmpty(std::chrono::duration<int64_t, std::milli> timeout);
};

template <typename T>
ThreadsafeQueue<T>::ThreadsafeQueue(std::queue<T> &&other)
    : queue{std::move(other)}, mutex{}, cv{} {};

template <typename T> std::queue<T> ThreadsafeQueue<T>::data() {
    const std::lock_guard lock(mutex);
    std::queue<T> temp{queue};
    return std::move(temp);
}

template <typename T> size_t ThreadsafeQueue<T>::size() {
    const std::lock_guard tmp(mutex);
    return queue.size();
};

template <typename T> bool ThreadsafeQueue<T>::empty() {
    const std::lock_guard tmp(mutex);
    return queue.empty();
};

template <typename T> T ThreadsafeQueue<T>::pop() {
    const std::lock_guard tmp(mutex);

    if (queue.empty()) {
        throw std::length_error(throw_message("Queue is empty()!"));
    }

    T ret = queue.front();
    queue.pop();

    return ret;
}

template <typename T> void ThreadsafeQueue<T>::push(T elem) {
    {
        const std::lock_guard tmp(mutex);
        queue.push(elem);
    }
    cv.notify_one();
}

template <typename T>
void ThreadsafeQueue<T>::waitOnNotEmpty(
    std::chrono::duration<int64_t, std::milli> timeout) {
    std::unique_lock<std::mutex> tmp(mutex);
    cv.wait_for(tmp, timeout, [this] { return !queue.empty(); });
}

template <typename T> void ThreadsafeQueue<T>::waitOnNotEmpty() {
    std::unique_lock<std::mutex> tmp(mutex);
    cv.wait(tmp, [this] { return !queue.empty(); });
}

} // namespace d19

#endif // AOC_D19_ATOMIC_QUEUE