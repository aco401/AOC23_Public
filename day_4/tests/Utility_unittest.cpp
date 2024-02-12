#include <gtest/gtest.h>
#include <utility.hpp>

TEST(all_test, VectorFromString) {
    std::vector<int> v{};
    std::string s{"98 19 95 93 62 76 36 70  1 55"};
    std::vector<int> c{98, 19, 95, 93, 62, 76, 36, 70, 1, 55};

    ns_day4::getVectorFromString(s, v);
    EXPECT_EQ(std::equal(v.begin(), v.end(), c.begin()), true);

    v.clear();
    s = " 98 19 95 93 62 76 36 70  1 55 ";
    ns_day4::getVectorFromString(s, v);
    EXPECT_EQ(std::equal(v.begin(), v.end(), c.begin()), true);

    v.clear();
    s = "98 19 95 93 62 76 36 70  1 55 ";
    ns_day4::getVectorFromString(s, v);
    EXPECT_EQ(std::equal(v.begin(), v.end(), c.begin()), true);

    v.clear();
    s = " 98 19 95 93 62 76 36 70  1 55";
    ns_day4::getVectorFromString(s, v);
    EXPECT_EQ(std::equal(v.begin(), v.end(), c.begin()), true);

    v.clear();
    s = " 98 19 95 62 76 36 70  1 55";
    ns_day4::getVectorFromString(s, v);
    EXPECT_EQ(std::equal(v.begin(), v.end(), c.begin()), false);
}