#include <gtest/gtest.h>

#include <DummyLogger.hpp>
#include <FileLogger.hpp>

#include <GraphAdjacentList.hpp>

#include <Algorithm.hpp>
#include <memory>

#include <Utility.hpp>

#include <VectorOfFile.hpp>

using namespace d17;

class FixtureVectorOfFile : public ::testing::Test {
  protected:
    void SetUp(void) {
        lineSize = 13;
        logger = std::make_shared<d17::FileLogger>("Test_VectorOfFile_log.txt");
        vecFile = std::make_unique<d17::VectorOfFile<unsigned>>(
            logger, "../src/data_example.txt");
    }

    void TearDown(void) {}

    std::shared_ptr<d17::ILog> logger;
    std::unique_ptr<d17::VectorOfFile<unsigned>> vecFile;
    unsigned lineSize;
};

TEST_F(FixtureVectorOfFile, blockSize) {
    logger->log("");
    logger->log("Test blockSize");

    EXPECT_EQ(vecFile->block_size(), lineSize);
}

TEST_F(FixtureVectorOfFile, size) {
    logger->log("");
    logger->log("Test size");

    EXPECT_EQ(vecFile->size(), 169);
}

TEST_F(FixtureVectorOfFile, DataRetrieval) {
    logger->log("");
    logger->log("Test DataRetrieval");

    EXPECT_EQ(vecFile->at(0), 2);
    EXPECT_EQ(vecFile->at(13), 3);
    EXPECT_EQ(vecFile->at(vecFile->size() - 1), 3);
}