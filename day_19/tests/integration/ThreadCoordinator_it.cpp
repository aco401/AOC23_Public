#include <gtest/gtest.h>

#include <logging/DummyLogger.hpp>
#include <logging/FileLogger.hpp>

#include <PartCoordinator.hpp>
#include <ThreadCoordinator.hpp>
#include <WorkflowStringParser.hpp>
#include <WorkflowThread.hpp>

#include <cstdlib>

using namespace d19;

class FixtureThreadCoordinator : public ::testing::Test {
  protected:
    void SetUp(void) {
        coordinatorThread = std::make_shared<d19::ThreadCoordinator>();
    }

    void TearDown(void) {}

    std::shared_ptr<d19::ThreadCoordinator> coordinatorThread;
};

TEST_F(FixtureThreadCoordinator, waitToStart_releaseStart) {

    std::atomic_bool milestone = false;
    std::atomic_bool created = false;

    std::thread t1{[&milestone, &created, this] {
        created = true;
        coordinatorThread->waitToStart();
        milestone = true;
    }};

    while (!created) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }; // Busy sleep wait

    EXPECT_FALSE(milestone);
    coordinatorThread->releaseStart();
    t1.join();
    EXPECT_TRUE(milestone);
}

TEST_F(FixtureThreadCoordinator, getStopResource_requestStop) {

    std::atomic_bool lever = false;
    std::atomic_bool started = false;

    std::thread t1{[&lever, &started, this] {
        auto stop = coordinatorThread->getStopResource();
        started = true;
        while (!stop.stop_requested()) {
            lever = true;
        }
    }};

    while (!started) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }; // Busy sleep wait
    lever = false;
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    EXPECT_TRUE(lever);
    lever = false;
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    EXPECT_TRUE(lever);

    coordinatorThread->requestStop();
    t1.join();
    lever = false;
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    EXPECT_FALSE(lever);
}