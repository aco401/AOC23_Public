#include <Day17Solver.hpp>
#include <gtest/gtest.h>

#include <DummyLogger.hpp>
#include <FileLogger.hpp>

using namespace d17;

class FixtureDay17Solver : public ::testing::Test {
  protected:
    void SetUp(void) {
        logger = std::make_shared<d17::FileLogger>("test_day17solver_log.txt");
    }

    void TearDown(void) {}

    std::shared_ptr<d17::ILog> logger;
};

TEST_F(FixtureDay17Solver, SolveFile) {
    logger->log("");
    logger->log("TEST: SolveFile");

    // Solve Advent of Code Day 17
    EXPECT_EQ(d17::solve_file(logger, "../src/data_example.txt", 0, 168), 102);
}