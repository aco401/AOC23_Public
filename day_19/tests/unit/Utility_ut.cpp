#include <gtest/gtest.h>

#include <Utility.hpp>
#include <map>
#include <vector>

using namespace d19;

class FixtureUtility : public ::testing::Test {
  protected:
    void SetUp(void) {}

    void TearDown(void) {}
};

// ------- unsigned findUnusedKey(std::vector<unsigned> &keys); -------

TEST_F(FixtureUtility, findUnusedKey_empty) {

    std::vector<unsigned> input{};

    EXPECT_EQ(findUnusedKey(input), 0);
}

TEST_F(FixtureUtility, findUnusedKey_first) {

    std::vector<unsigned> input{1, 2, 3, 4, 5, 6};

    EXPECT_EQ(findUnusedKey(input), 0);
}

TEST_F(FixtureUtility, findUnusedKey_last) {

    std::vector<unsigned> input{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    EXPECT_EQ(findUnusedKey(input), 10);
}

TEST_F(FixtureUtility, findUnusedKey_middle) {

    std::vector<unsigned> input{0, 1, 2, 3, 4, 6, 7, 8, 9};

    EXPECT_EQ(findUnusedKey(input), 5);
}

TEST_F(FixtureUtility, findUnusedKeyTemplate_empty) {

    std::map<unsigned, unsigned> input{};

    EXPECT_EQ(findUnusedKey(input), 0);
}

TEST_F(FixtureUtility, findUnusedKeyTemplate_first) {

    std::map<unsigned, unsigned> input{{1, 321}, {2, 2152}, {3, 21321},
                                       {4, 217}, {5, 2131}, {6, 1092383}};

    EXPECT_EQ(findUnusedKey(input), 0);
}

TEST_F(FixtureUtility, findUnusedKeyTemplate_last) {

    std::map<unsigned, unsigned> input{
        {0, 4},    {1, 321},     {2, 2152}, {3, 21321}, {4, 217},
        {5, 2131}, {6, 1092383}, {7, 217},  {8, 2131},  {9, 1092383}};

    EXPECT_EQ(findUnusedKey(input), 10);
}

TEST_F(FixtureUtility, findUnusedKeyTemplate_middle) {

    std::map<unsigned, unsigned> input{
        {0, 1},    {1, 321},      {2, 2152}, {3, 21321}, {4, 217},
        {5, 2131}, {10, 1092383}, {7, 217},  {8, 2131},  {9, 1092383}};

    EXPECT_EQ(findUnusedKey(input), 6);
}