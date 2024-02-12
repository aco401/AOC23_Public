#include <DummyLogger.hpp>
#include <ScratchCard.hpp>
#include <gtest/gtest.h>
#include <memory>

// Demonstrate some basic assertions.
TEST(all_test, BasicAssertions) {
    // Same elem number in both vectors
    std::shared_ptr<ns_day4::ILogger> logger =
        std::make_shared<ns_day4::DummyLogger>();

    EXPECT_EQ(ScratchCard(logger, std::vector<int>{1}, std::vector<int>{1})
                  .calculatePoints(),
              1);

    EXPECT_EQ(ScratchCard(logger, std::vector<int>{1}, std::vector<int>{0})
                  .calculatePoints(),
              0);

    // Vector Size Difference
    EXPECT_EQ(
        ScratchCard(logger, std::vector<int>{1, 2, 3, 4}, std::vector<int>{1})
            .calculatePoints(),
        1);

    EXPECT_EQ(
        ScratchCard(logger, std::vector<int>{1}, std::vector<int>{1, 2, 3, 4})
            .calculatePoints(),
        1);

    // Multiple Points on a ScratchCard
    EXPECT_EQ(ScratchCard(logger, std::vector<int>{1, 2, 3, 4},
                          std::vector<int>{1, 2, 3, 4})
                  .calculatePoints(),
              8);

    EXPECT_EQ(ScratchCard(logger, std::vector<int>{1, 10, 20, 30, 50, 60, 70},
                          std::vector<int>{60, 10})
                  .calculatePoints(),
              2);
}

TEST(all_test, TypicalCard) {
    // Same Elem in both vectors
    std::shared_ptr<ns_day4::ILogger> logger =
        std::make_shared<ns_day4::DummyLogger>();

    ScratchCard card{
        logger, std::vector<int>{69, 12, 75, 19, 83, 56, 73, 53, 52, 91},
        std::vector<int>{83, 63, 56, 30, 77, 94, 12, 81, 42, 69, 52, 70, 6,
                         97, 20, 43, 61, 22, 75, 19, 73, 32, 74, 53, 91}};

    EXPECT_EQ(card.calculatePoints(), 512);
}
