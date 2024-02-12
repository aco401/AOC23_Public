#include <ThreadsafeQueue.hpp>
#include <gtest/gtest.h>

#include <ElapsedTimer.hpp>
#include <chrono>

using namespace d19;

class FixtureThreadsafeQueue : public ::testing::Test {
  protected:
    void SetUp(void) {}

    void TearDown(void) {}

    d19::ThreadsafeQueue<unsigned> queue;
};

TEST_F(FixtureThreadsafeQueue, PushAndPop) {
    queue.push(1);
    EXPECT_EQ(queue.pop(), 1);
}

TEST_F(FixtureThreadsafeQueue, Empty) {
    EXPECT_TRUE(queue.empty());
    queue.push(1);
    EXPECT_FALSE(queue.empty());
    queue.pop();
    EXPECT_TRUE(queue.empty());
}

TEST_F(FixtureThreadsafeQueue, Size) {
    EXPECT_EQ(queue.size(), 0);
    queue.push(1);
    EXPECT_EQ(queue.size(), 1);
    queue.pop();
    EXPECT_EQ(queue.size(), 0);
    queue.push(500000);
    queue.push(321321);
    EXPECT_EQ(queue.size(), 2);
    queue.pop();
    EXPECT_EQ(queue.size(), 1);
    queue.pop();
    EXPECT_EQ(queue.size(), 0);
}

TEST_F(FixtureThreadsafeQueue, EmptyExceptionDefault) {

    EXPECT_THROW(queue.pop(), std::length_error);
}

TEST_F(FixtureThreadsafeQueue, EmptyExceptionUsed) {

    std::vector<unsigned> vec{4,   43, 432, 53, 25342, 564, 6,  5476,
                              547, 6,  86,  3,  32,    432, 432};
    for (const auto &elem : vec) {
        queue.push(elem);
    }

    for (size_t i = 0; i < vec.size(); i++) {
        EXPECT_EQ(queue.pop(), vec.at(i));
    }

    EXPECT_THROW(queue.pop(), std::length_error);
}

TEST_F(FixtureThreadsafeQueue, data) {

    queue.push(7);
    queue.push(9);
    queue.push(11);

    d19::ThreadsafeQueue<unsigned> queue_local{queue.data()};

    // Remove all queue - to exclude possibility of referring to same object
    queue.pop();
    queue.pop();
    queue.pop();

    EXPECT_EQ(queue_local.pop(), 7);
    EXPECT_EQ(queue_local.pop(), 9);
    EXPECT_EQ(queue_local.pop(), 11);
}