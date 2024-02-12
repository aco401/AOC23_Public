#include <Day19Solver.hpp>
#include <gtest/gtest.h>

#include <logging/DummyLogger.hpp>
#include <logging/FileLogger.hpp>

#include <algorithm>

using namespace d19;

const char *data_input{"../src/data_input.txt"};
const char *data_example{"../src/data_example.txt"};

class FixtureDay19Solver : public ::testing::Test {
  protected:
    void SetUp(void) {
        //logger = std::make_shared<d19::FileLogger>("test_day19solver_log.txt");
        logger = std::make_shared<d19::DummyLogger>();
    }

    void TearDown(void) {}

    std::shared_ptr<d19::ILog> logger;
};

TEST_F(FixtureDay19Solver, SolveFileExample) {

    // Solve Advent of Code Day 19
    d19::AOCSolver solver(logger, data_example);
    EXPECT_EQ(solver.sort(1, nullptr), 19114);
    EXPECT_EQ(solver.sort(2, nullptr), 19114);
    EXPECT_EQ(solver.sort(3, nullptr), 19114);
    EXPECT_EQ(solver.sort(4, nullptr), 19114);
    EXPECT_EQ(solver.sort(5, nullptr), 19114);
    //EXPECT_EQ(d19::sort(logger, data_example, 11), 19114);
}

TEST_F(FixtureDay19Solver, SolveFileInput) {

    // Solve Advent of Code Day 19
    d19::AOCSolver solver(logger, data_input);
    EXPECT_EQ(solver.sort(1, nullptr), 348378);
    EXPECT_EQ(solver.sort(3, nullptr), 348378);
    EXPECT_EQ(solver.sort(11, nullptr), 348378);
}

// TODO Test all other smaller functions
