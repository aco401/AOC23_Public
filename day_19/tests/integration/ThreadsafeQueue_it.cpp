#include <ThreadsafeQueue.hpp>
#include <gtest/gtest.h>

#include <logging/DummyLogger.hpp>
#include <logging/FileLogger.hpp>

#include <ElapsedTimer.hpp>
#include <atomic>
#include <chrono>
#include <thread>

using namespace d19;

class FixtureThreadsafeQueue : public ::testing::Test {
  protected:
    void SetUp(void) {}

    void TearDown(void) {}

    d19::ThreadsafeQueue<unsigned> queue;
};

TEST_F(FixtureThreadsafeQueue, DataRaceProvocation) {

    std::atomic_bool start = false;

    std::vector<std::thread> vecT{};
    unsigned amountThreads = 10;
    unsigned numInserts = 10;
    for (size_t i = 0; i < amountThreads; ++i) {

        vecT.emplace_back([&start, this, numInserts] {
            while (!start) {
            }; // SpinLock
            for (size_t i = 0; i < numInserts; ++i) {
                queue.push(1);
            }
        });
    }

    start = true;
    for (auto it = vecT.begin(); it != vecT.end(); ++it) {
        it->join();
    }

    EXPECT_EQ(queue.size(), numInserts * amountThreads);
}

TEST_F(FixtureThreadsafeQueue, WaitOnNotEmpty) {

    unsigned waitTime = 20;
    std::atomic_bool waiting = false;
    d19::ElapsedTimer timer{};

    std::thread t1{[waitTime, &waiting, this] {
        std::this_thread::sleep_for(std::chrono::milliseconds(waitTime + 1));
        waiting = true;
        queue.push(1); // to wake up main thread
    }};

    timer.start();
    queue.waitOnNotEmpty();
    timer.stop();
    t1.join();

    if (!waiting) {
        EXPECT_TRUE(false);
    }

    EXPECT_TRUE(timer.getElapsedTime() > std::chrono::milliseconds(waitTime));
}

TEST_F(FixtureThreadsafeQueue, WaitOnNotEmpty_timeout) {

    d19::ElapsedTimer timer{};

    std::chrono::milliseconds waitTime{20};

    timer.start();
    queue.waitOnNotEmpty(waitTime + std::chrono::milliseconds(1));
    timer.stop();

    EXPECT_TRUE(timer.getElapsedTime() > waitTime);
}

TEST_F(FixtureThreadsafeQueue, WaitOnNotEmpty_Destruction) {
    // case: shared ThreadSafeQueue in global scope
    // 1 threads waits on elements, but suddenly queue is deleted

    // "new" used to call delete explicitly
    d19::ThreadsafeQueue<unsigned> *queue_local_ptr =
        new d19::ThreadsafeQueue<unsigned>();

    std::thread t1{[queue_local_ptr] {
        queue_local_ptr->waitOnNotEmpty();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        queue_local_ptr->waitOnNotEmpty(); // here exception?
    }};

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    delete queue_local_ptr;

    t1.join();
}
