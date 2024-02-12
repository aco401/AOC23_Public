#include <Day5Solver.hpp>
#include <gtest/gtest.h>

#include <DummyLogger.hpp>
#include <FileLogger.hpp>

#include <CategoryMapLocalStorage.hpp>

using namespace ns_day_5;

class FixtureICategoryMap : public ::testing::Test {
  protected:
    void SetUp(void) {
        logger =
            std::make_shared<ns_day_5::FileLogger>("Test_ICategoryMap_log.txt");
    }

    void TearDown(void) {}

    std::shared_ptr<ns_day_5::ILog> logger;
};

TEST_F(FixtureICategoryMap, TestFindDestination) {
    const std::vector<int> seeds{79, 14, 55, 13};
    const std::vector<int> soilOfSeeds{81, 14, 57, 13};
    const std::vector<int> fertilizerOfSoil{81, 53, 57, 52};
    const std::vector<int> WaterOfFertilizer{81, 49, 53, 41};

    const ns_day_5::CategoryMapInputFactory::ParseType parseType =
        ns_day_5::CategoryMapInputFactory::ParseType::ConstArray;

    ns_day_5::CategoryMapInputFactory factory{logger};

    // Test SeedToSoil - mapping
    std::unique_ptr<ns_day_5::ICategoryMap> categoryMap =
        std::make_unique<ns_day_5::CategoryMapLocalStorage>(
            logger,
            factory.createInput(ns_day_5::CategoryMapEnum::Type::SeedToSoil,
                                parseType));

    for (size_t i = 0; i < seeds.size(); i++) {
        EXPECT_EQ(categoryMap->findDestination(seeds[i]), soilOfSeeds[i]);
    }

    // Test SoilToFertilizer - mapping
    categoryMap = std::make_unique<ns_day_5::CategoryMapLocalStorage>(
        logger,
        factory.createInput(ns_day_5::CategoryMapEnum::Type::SoilToFertilizer,
                            parseType));

    for (size_t i = 0; i < soilOfSeeds.size(); i++) {
        EXPECT_EQ(categoryMap->findDestination(soilOfSeeds[i]),
                  fertilizerOfSoil[i]);
    }

    // Test FertilizerToWater - mapping
    categoryMap = std::make_unique<ns_day_5::CategoryMapLocalStorage>(
        logger,
        factory.createInput(ns_day_5::CategoryMapEnum::Type::FertilizerToWater,
                            parseType));

    for (size_t i = 0; i < fertilizerOfSoil.size(); i++) {
        EXPECT_EQ(categoryMap->findDestination(fertilizerOfSoil[i]),
                  WaterOfFertilizer[i]);
    }
}