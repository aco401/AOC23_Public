#include <gtest/gtest.h>

#include <DummyLogger.hpp>
#include <FileLogger.hpp>

#include <GraphFileHandler.hpp>

using namespace d17;

constexpr const char *filename_data_input = "../src/data_input.txt";
constexpr const char *filename_data_example = "../src/data_example.txt";

class FixtureGraphFileHandler : public ::testing::Test {
  protected:
    void SetUp(void) {
        logger =
            std::make_shared<d17::FileLogger>("test_GraphFileHandler_log.txt");
        graphFactory = std::make_unique<d17::GraphFileHandler>(logger);
        //filename_data_input = "../src/data_input.txt"; // TODO usage results in not beeing able to open a file
    }

    void TearDown(void) {}

    std::shared_ptr<d17::ILog> logger;
    std::unique_ptr<d17::GraphFileHandler> graphFactory;

    //std::string filename_data_input;
};

TEST_F(FixtureGraphFileHandler, TestNodeComparison) {
    logger->log("Test NodeComparison");

    d17::IGraph::Node node1{.destinationId = 0,
                            .cost = 2,
                            .dir = d17::IGraph::NeighbourDirection::DOWN};
    d17::IGraph::Node node2{.destinationId = 0,
                            .cost = 2,
                            .dir = d17::IGraph::NeighbourDirection::DOWN};

    EXPECT_TRUE(node1 == node2);
}

TEST_F(FixtureGraphFileHandler, TestCreateGraph) {
    logger->log("Test CreateGraph");

    std::unique_ptr<d17::IGraph> graph =
        graphFactory->createGraph("../src/data_example.txt");

    EXPECT_EQ(graph->size(), 169);
}

TEST_F(FixtureGraphFileHandler, TestCreateGraphAccessNodes) {
    logger->log("Test TestCreateGraphAccessNodes");

    std::unique_ptr<d17::IGraph> graph =
        graphFactory->createGraph("../src/data_example.txt");
    std::vector<d17::IGraph::Node>::iterator it;

    std::vector<unsigned> expectedNeighbors{167, 155};

    for (it = graph->getNeighbours(168, d17::IGraph::IteratorPos::Start);
         it != graph->getNeighbours(168, d17::IGraph::IteratorPos::End); it++) {
        EXPECT_EQ(std::find(expectedNeighbors.begin(), expectedNeighbors.end(),
                            it->destinationId) != expectedNeighbors.end(),
                  true);

        logger->log(it->destinationId);
    }
}

TEST_F(FixtureGraphFileHandler, TestGraphCost) {
    logger->log("Test TestCreateGraphAccessNodes");

    std::unique_ptr<d17::IGraph> graph =
        graphFactory->createGraph("../src/data_example.txt");
    std::vector<d17::IGraph::Node>::iterator it;

    std::vector<unsigned> expectedNeighbors{167, 155};

    for (it = graph->getNeighbours(168, d17::IGraph::IteratorPos::Start);
         it != graph->getNeighbours(168, d17::IGraph::IteratorPos::End); it++) {
        if (it->destinationId == 167) {
            EXPECT_EQ(it->cost, 3);
        }

        if (it->destinationId == 155) {
            EXPECT_EQ(it->cost, 5);
        }
    }

    for (it = graph->getNeighbours(1, d17::IGraph::IteratorPos::Start);
         it != graph->getNeighbours(1, d17::IGraph::IteratorPos::End); it++) {
        if (it->destinationId == 2) {
            EXPECT_EQ(it->cost, 1);
        }

        if (it->destinationId == 15) {
            EXPECT_EQ(it->cost, 2);
        }
    }
}
