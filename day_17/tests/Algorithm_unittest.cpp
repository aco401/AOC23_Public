#include <Algorithm.hpp>
#include <gtest/gtest.h>

#include <Day17Solver.hpp>
#include <DummyLogger.hpp>
#include <FileLogger.hpp>
#include <GraphFileHandler.hpp>
#include <Utility.hpp>

using namespace d17;

class FixtureAlgorithm : public ::testing::Test {
  protected:
    void SetUp(void) {
        filename = "../src/data_example.txt";
        logger = std::make_shared<d17::FileLogger>("test_day17solver_log.txt");
        graphFactory = std::make_unique<d17::GraphFileHandler>(logger);
        graph = graphFactory->createGraph(filename);
    }

    void TearDown(void) {}

    std::string filename;
    std::shared_ptr<d17::ILog> logger;
    std::unique_ptr<d17::GraphFileHandler> graphFactory;
    std::unique_ptr<d17::IGraph> graph;
};

TEST_F(FixtureAlgorithm, TestCostCalculation) {
    logger->log("");
    logger->log("TEST: CostCalculation");

    unsigned startId = 0;
    unsigned targetId = 168;

    std::pair<std::list<unsigned>, unsigned> ret =
        d17::algorithm::dijkstraGraph(logger, *graph.get(), startId, targetId);

    logger->log("Path: " + d17::listToString(ret.first));
    logger->log("Cost from Node " + std::to_string(startId) + " to " +
                std::to_string(targetId) + ": " + std::to_string(ret.second));

    struct Sum {
        void operator()(unsigned elem) {
            if (elem != 0) {
                static VectorOfFile<unsigned> vec("../src/data_example.txt");
                sum += vec.at(elem);
            }
        };
        unsigned sum{0};
    };

    // Calculate cost
    Sum sum = std::for_each(ret.first.begin(), ret.first.end(), Sum());

    // Solve Advent of Code Day 17
    EXPECT_EQ(d17::solve_file(logger, filename, 0, 168), sum.sum);
}