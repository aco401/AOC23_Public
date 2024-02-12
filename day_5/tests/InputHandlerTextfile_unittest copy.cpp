#include <Day5Solver.hpp>
#include <gtest/gtest.h>

#include <DummyLogger.hpp>
#include <FileLogger.hpp>

#include <InputHandlerTextfile.hpp>

using namespace ns_day_5;

constexpr const char *filename_data_input = "../src/data_input.txt";
constexpr const char *filename_data_example = "../src/data_example.txt";

class FixtureInputHandlerTextfile : public ::testing::Test {
  protected:
    void SetUp(void) {
        logger =
            std::make_shared<ns_day_5::FileLogger>("test_InputHandler_log.txt");
        //filename_data_input = "../src/data_input.txt"; // TODO usage results in not beeing able to open a file
    }

    void TearDown(void) {}

    std::shared_ptr<ns_day_5::ILog> logger;
    //std::string filename_data_input;
};

TEST_F(FixtureInputHandlerTextfile, TestElementsOfMapping) {
    logger->log("TestElementsOfMapping");

    // SeedToSoil
    InputHandlerTextfile handler{logger,
                                 ns_day_5::CategoryMapEnum::Type::SeedToSoil,
                                 filename_data_input};

    // Test the elements of a textfile vector
    EXPECT_EQ(handler.elementsOfMapping(), 51);

    // // SoilToFertilizer
    // handler = InputHandlerTextfile(
    //     logger, ns_day_5::CategoryMapEnum::Type::SoilToFertilizer, filename_data_input);

    // // Test the elements of a textfile vector
    // EXPECT_EQ(handler.elementsOfMapping(), 51);

    // // FertilizerToWater
    // handler = InputHandlerTextfile(
    //     logger, ns_day_5::CategoryMapEnum::Type::FertilizerToWater, filename_data_input);

    // // Test the elements of a textfile vector
    // EXPECT_EQ(handler.elementsOfMapping(), 51);

    // WaterToLight
    handler = InputHandlerTextfile(
        logger, ns_day_5::CategoryMapEnum::Type::WaterToLight,
        filename_data_input);

    // Test the elements of a textfile vector
    EXPECT_EQ(handler.elementsOfMapping(), 24 * 3);

    // // LightToTemperature
    // handler = InputHandlerTextfile(
    //     logger, ns_day_5::CategoryMapEnum::Type::LightToTemperature, filename_data_input);

    // // Test the elements of a textfile vector
    // EXPECT_EQ(handler.elementsOfMapping(), 51);

    // // TemperatureToHumidity
    // handler = InputHandlerTextfile(
    //     logger, ns_day_5::CategoryMapEnum::Type::TemperatureToHumidity,
    //     filename_data_input);

    // // Test the elements of a textfile vector
    // EXPECT_EQ(handler.elementsOfMapping(), 51);

    // // HumidityToLocation
    // handler = InputHandlerTextfile(
    //     logger, ns_day_5::CategoryMapEnum::Type::HumidityToLocation, filename_data_input);

    // // Test the elements of a textfile vector
    // EXPECT_EQ(handler.elementsOfMapping(), 51);
}

TEST_F(FixtureInputHandlerTextfile, TestElementsOfMappingExampleFile) {
    logger->log("TestElementsOfMappingExampleFile");

    // SeedToSoil
    InputHandlerTextfile handler{logger,
                                 ns_day_5::CategoryMapEnum::Type::SeedToSoil,
                                 filename_data_example};

    // Test the elements of a textfile vector
    EXPECT_EQ(handler.elementsOfMapping(), 6);

    // WaterToLight
    handler = InputHandlerTextfile(
        logger, ns_day_5::CategoryMapEnum::Type::WaterToLight,
        filename_data_example);

    // Test the elements of a textfile vector
    EXPECT_EQ(handler.elementsOfMapping(), 6);

    // fertilizer-to-water
    handler = InputHandlerTextfile(
        logger, ns_day_5::CategoryMapEnum::Type::FertilizerToWater,
        filename_data_example);

    // Test the elements of a textfile vector
    EXPECT_EQ(handler.elementsOfMapping(), 12);
}

TEST_F(FixtureInputHandlerTextfile, TestValueOfIndex) {
    logger->log("TestValueOfIndex");

    InputHandlerTextfile handler{logger,
                                 ns_day_5::CategoryMapEnum::Type::SeedToSoil,
                                 filename_data_input};

    // Test reading the elements of a textfile vector
    EXPECT_EQ(handler.getValueOfIndex(3).value_or(0), 3584439879);
    EXPECT_EQ(handler.getValueOfIndex(3).value_or(0), 3584439879);
    EXPECT_EQ(handler.getValueOfIndex(0).value_or(0), 3534435790);
    EXPECT_EQ(handler.getValueOfIndex(30).value_or(0), 3285153612);
}

TEST_F(FixtureInputHandlerTextfile, getRowAtPtrDataInput) {
    logger->log("getRowAtPtrDataInput");

    InputHandlerTextfile handler{logger,
                                 ns_day_5::CategoryMapEnum::Type::SeedToSoil,
                                 filename_data_input};

    ns_day_5::ICategoryMapInputHandler::InputRow row = handler.getRowAtPtr();
    EXPECT_EQ(row.destination, 3534435790);
    EXPECT_EQ(row.source, 4123267198);
    EXPECT_EQ(row.length, 50004089);
}

TEST_F(FixtureInputHandlerTextfile, TestValueOfIndexExample) {
    logger->log("TestValueOfIndexExample");

    InputHandlerTextfile handler{logger,
                                 ns_day_5::CategoryMapEnum::Type::SeedToSoil,
                                 filename_data_example};

    // Test reading the elements of a textfile vector
    EXPECT_EQ(handler.getValueOfIndex(2).value_or(0), 2);
}

TEST_F(FixtureInputHandlerTextfile, getRowAtPtrDataExample) {
    logger->log("getRowAtPtrDataExample");

    InputHandlerTextfile handler{logger,
                                 ns_day_5::CategoryMapEnum::Type::SeedToSoil,
                                 filename_data_example};

    ns_day_5::ICategoryMapInputHandler::InputRow row = handler.getRowAtPtr();
    EXPECT_EQ(row.destination, 50);
    EXPECT_EQ(row.source, 98);
    EXPECT_EQ(row.length, 2);

    handler.movePtr(ns_day_5::ICategoryMapInputHandler::PtrMovement::Next);

    row = handler.getRowAtPtr();
    EXPECT_EQ(row.destination, 52);
    EXPECT_EQ(row.source, 50);
    EXPECT_EQ(row.length, 48);
}