#include <Day5Solver.hpp>
#include <gtest/gtest.h>

#include <DummyLogger.hpp>
#include <FileLogger.hpp>

#include <CategoryMapLocalStorage.hpp>

using namespace ns_day_5;

class FixtureDay5Solver : public ::testing::Test {
  protected:
    void SetUp(void) {
        logger =
            std::make_shared<ns_day_5::FileLogger>("test_day5_solver_log.txt");
    }

    void TearDown(void) {}

    std::shared_ptr<ns_day_5::ILog> logger;
};

TEST_F(FixtureDay5Solver, SolveFile) {
    logger->log("");
    logger->log("TEST: SolveFile");

    // Solve Advent of Code Day 5
    EXPECT_EQ(ns_day_5::solve_file(logger, "../src/data_example.txt").second,
              35); // TODO: Problem solveFile still uses const map of seeds

    EXPECT_EQ(
        ns_day_5::solve_file(logger, "../src/data_input.txt").second,
        35); // TODO: Problem solveFile still uses const map of seeds - should be solved with the usage of TextfileArray
}

TEST_F(FixtureDay5Solver, SolveExample) {
    logger->log("");
    logger->log("TEST: SolveExample");
    // Solve Advent of Code Day 5
    EXPECT_EQ(ns_day_5::solve_example(logger).second, 35);
}

TEST(all_test, TestFindLocationForSeed) { // TODO might show error

    // const std::vector<int> seeds{79, 14, 55, 13};
    // const std::vector<int> soilOfSeeds{81, 14, 57, 13};

    // const ns_day_5::CategoryMapInputFactory::ParseType parseType =
    //     ns_day_5::CategoryMapInputFactory::ParseType::ConstArray;

    // std::shared_ptr<ns_day_5::ILog> logger =
    //     std::make_shared<ns_day_5::DummyLogger>();
    // ns_day_5::CategoryMapInputFactory factory{logger};

    // // Acquire all CategoryMaps (handlers)
    // std::map<ns_day_5::CategoryMapEnum::Type,
    //          std::unique_ptr<ns_day_5::ICategoryMap>>
    //     maps{};
    // for (auto mapping : ns_day_5::CategoryMapEnum::All) {
    //     maps.insert(
    //         {mapping, std::make_unique<ns_day_5::CategoryMapLocalStorage>(
    //                       logger, factory.createInput(mapping, parseType))});
    // };

    // // For each seed find location
    // std::map<int, int> seedToLocation =
    //     ns_day_5::findLocationForSeed(seeds, maps);

    // // Find lowest location
    // std::pair<int, int> lowest = ns_day_5::findLowestLocation(seedToLocation);

    // // Solve Advent of Code Day 4
    // EXPECT_EQ(lowest.second, 35);
}

TEST_F(FixtureDay5Solver, TestFindLowestLocation) {
    logger->log("");
    logger->log("TEST: TestFindLowestLocation");
    ns_day_5::CategoryMapInputFactory factory{logger};

    std::map<int, int> seedToLocation{{0, 30}, {1, 31}, {30, 20}};

    // Find lowest location
    std::pair<int, int> lowest =
        ns_day_5::findLowestLocation(logger, seedToLocation);
    EXPECT_EQ(lowest.second, 20);

    seedToLocation = {{0, -30}, {1, -31}, {-30, 20}};
    lowest = ns_day_5::findLowestLocation(logger, seedToLocation);
    EXPECT_EQ(lowest.second, -31);
}