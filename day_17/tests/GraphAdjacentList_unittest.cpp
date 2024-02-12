#include <gtest/gtest.h>

#include <DummyLogger.hpp>
#include <FileLogger.hpp>

#include <GraphAdjacentList.hpp>

#include <Algorithm.hpp>
#include <memory>

#include <Utility.hpp>

using namespace d17;

class FixtureGraphAdjacentList : public ::testing::Test {
  protected:
    void SetUp(void) {
        size = 9;
        logger =
            std::make_shared<d17::FileLogger>("Test_GraphAdjacentList_log.txt");
        graph = std::make_unique<d17::GraphAdjacentList>(logger, size);

        graph->addEdge(0, 1, 4, d17::IGraph::NeighbourDirection::UP);
        graph->addEdge(0, 2, 23, d17::IGraph::NeighbourDirection::DOWN);
        graph->addEdge(0, 3, 100, d17::IGraph::NeighbourDirection::RIGHT);
        graph->addEdge(1, 4, 45, d17::IGraph::NeighbourDirection::RIGHT);
        graph->addEdge(2, 5, 1, d17::IGraph::NeighbourDirection::RIGHT);
        graph->addEdge(5, 3, 37, d17::IGraph::NeighbourDirection::UP);
        graph->addEdge(4, 3, 20, d17::IGraph::NeighbourDirection::DOWN);

        graph->addEdge(3, 5, 38, d17::IGraph::NeighbourDirection::DOWN);
        graph->addEdge(3, 8, 100, d17::IGraph::NeighbourDirection::RIGHT);
        graph->addEdge(5, 6, 1, d17::IGraph::NeighbourDirection::RIGHT);
        graph->addEdge(6, 7, 2, d17::IGraph::NeighbourDirection::RIGHT);
        graph->addEdge(7, 8, 2, d17::IGraph::NeighbourDirection::UP);
    }

    void TearDown(void) {}

    std::shared_ptr<d17::ILog> logger;
    std::unique_ptr<d17::GraphAdjacentList> graph;
    unsigned size;
};

TEST_F(FixtureGraphAdjacentList, size) {
    logger->log("");
    logger->log("Testsize");

    EXPECT_EQ(graph->size(), size);
}

TEST_F(FixtureGraphAdjacentList, DataRetrieval) {
    logger->log("");
    logger->log("TestDataRetrieval");

    EXPECT_EQ(
        graph->getNeighbours(0, d17::IGraph::IteratorPos::Start)->destinationId,
        1);
    EXPECT_EQ(graph->getNeighbours(1, d17 ::IGraph::IteratorPos::Start)
                  ->destinationId,
              4);
}

TEST_F(FixtureGraphAdjacentList, TestFindShortestPath) {
    logger->log("");
    logger->log("TestFindShortestPath");

    std::pair<std::list<unsigned>, unsigned> ret =
        d17::algorithm::dijkstraGraph(logger, *graph.get(), 0, 3);
    logger->log(d17::listToString(ret.first));

    EXPECT_EQ(ret.second, 61);

    std::list<unsigned> evalList{0, 2, 5, 3};
    for (size_t i = 0; i < evalList.size(); i++) {
        EXPECT_EQ(ret.first.front(), evalList.front());
        ret.first.pop_front();
        evalList.pop_front();
    }
}

TEST_F(FixtureGraphAdjacentList, TestFindShortestPathStraightLine) {
    logger->log("");
    logger->log("TestFindShortestPath");

    std::pair<std::list<unsigned>, unsigned> ret =
        d17::algorithm::dijkstraGraph(logger, *graph.get(), 0, 8);
    logger->log(d17::listToString(ret.first));

    EXPECT_EQ(ret.second, 111);

    std::list<unsigned> evalList{0, 1, 4, 3, 5, 6, 7, 8};
    for (size_t i = 0; i < evalList.size(); i++) {
        EXPECT_EQ(ret.first.front(), evalList.front());
        ret.first.pop_front();
        evalList.pop_front();
    }
}