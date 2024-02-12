#include <exception>
#include <gtest/gtest.h>
#include <measuring/ElapsedTimer.hpp>
#include <thread>

#include <logging/DummyLogger.hpp>
#include <logging/FileLogger.hpp>

using namespace d19;

class FixtureElapsedTimer : public ::testing::Test {
  protected:
    void SetUp(void) {}

    void TearDown(void) {}

    d19::ElapsedTimer timer{};
};

TEST_F(FixtureElapsedTimer, AfterInit) {

    EXPECT_THROW(timer.getElapsedTime(), std::domain_error);
}

TEST_F(FixtureElapsedTimer, ElapsedTimeNormalUsage) {

    timer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    timer.stop();

    EXPECT_TRUE(timer.getElapsedTime() >= std::chrono::milliseconds(100));
    timer.clear();
    EXPECT_THROW(timer.getElapsedTime(), std::domain_error);
}

TEST_F(FixtureElapsedTimer, ElapsedTimeNormalUsage2Times) {

    timer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    timer.stop();

    EXPECT_TRUE(timer.getElapsedTime() >= std::chrono::milliseconds(100));
    timer.clear();
    EXPECT_THROW(timer.getElapsedTime(), std::domain_error);
    timer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    timer.stop();
    EXPECT_TRUE(timer.getElapsedTime() >= std::chrono::milliseconds(100));
}

TEST_F(FixtureElapsedTimer, WrongUsageOnlyStart) {

    timer.start();

    EXPECT_THROW(timer.getElapsedTime(), std::domain_error);
}

TEST_F(FixtureElapsedTimer, WrongUsageOnlyStop) {

    timer.stop();

    EXPECT_THROW(timer.getElapsedTime(), std::domain_error);
}

TEST_F(FixtureElapsedTimer, DoubleStart) {

    timer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    timer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    timer.stop();

    EXPECT_THROW(timer.getElapsedTime(), std::domain_error);
}

TEST_F(FixtureElapsedTimer, WrongOrder) {

    timer.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    timer.start();

    EXPECT_THROW(timer.getElapsedTime(), std::domain_error);
}