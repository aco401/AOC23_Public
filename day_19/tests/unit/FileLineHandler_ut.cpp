#include <cassert>
#include <gtest/gtest.h>

#include <logging/DummyLogger.hpp>
#include <logging/FileLogger.hpp>

#include <memory>

#include <Utility.hpp>

#include <filehandling/FileLineHandler.hpp>
#include <string_view>
#include <vector>

using namespace d19;

const char *input_file{"../tests/unit/data_input_test.txt"};
const char *input_file_delimiter{
    "../tests/unit/data_input_test_other_delimiter.txt"};

class FixtureFileLineHandler : public ::testing::Test {
  protected:
    void SetUp(void) {
        //logger = std::make_shared<d19::FileLogger>("Test_FileLineHandler_log.txt");
        logger = std::make_shared<d19::DummyLogger>();
        linehandler =
            std::make_unique<d19::FileLineHandler>(logger, input_file, '\n', 0);
        assert(linehandler);
        assert(logger);
    }

    void TearDown(void) {}

    std::shared_ptr<d19::ILog> logger;
    std::unique_ptr<d19::FileLineHandler> linehandler;
};

TEST_F(FixtureFileLineHandler, TestPosBeforegetLine) {
    std ::ifstream stream{input_file};

    std::string currLine{};

    stream.seekg(33, std::ios_base::beg);
    EXPECT_EQ(stream.peek(), '\n');

    std::getline(stream, currLine, '\n');
    EXPECT_EQ(currLine, "");

    // After a getline the cursor pos jumps to start of next line
    EXPECT_EQ(stream.tellg(), 34);

    EXPECT_EQ(stream.peek(), 'q');
}

TEST_F(FixtureFileLineHandler, TestPosAftergetLine) {
    std ::ifstream stream{input_file};

    std::string currLine{};

    stream.seekg(0, std::ios_base::beg);

    std::getline(stream, currLine, '\n');

    // After a getline the cursor pos jumps to start of next line
    EXPECT_EQ(stream.tellg(), 34);

    EXPECT_EQ(stream.peek(), 'q');
}

TEST_F(FixtureFileLineHandler, TestSepLine) {
    std ::ifstream stream{input_file};

    std::string currLine{};

    stream.seekg(0, std::ios_base::beg);

    std::getline(stream, currLine, '\n');
    std::getline(stream, currLine, '\n');
    std::getline(stream, currLine, '\n');

    EXPECT_EQ(currLine, "");
    EXPECT_EQ(currLine.size(), 0);

    EXPECT_EQ(stream.peek(), 'x');
}

TEST_F(FixtureFileLineHandler, IteratorEndOfVector) {
    std::vector<unsigned> vec{9, 9, 9, 54, 546, 4};

    EXPECT_EQ(*(--vec.end()), 4);
}

TEST_F(FixtureFileLineHandler, Init) {

    EXPECT_EQ(*linehandler->begin(), "fmz{x<2152:R,s>3520:A,a<3391:A,A}");
    EXPECT_EQ(*(--linehandler->end()), "qhh{a>2045:A,s>3259:A,R}");
}

TEST_F(FixtureFileLineHandler, Index) {

    linehandler =
        std::make_unique<d19::FileLineHandler>(logger, input_file, '\n', 1);

    EXPECT_EQ(*linehandler->begin(), "xh{m<1065:bxz,spk}");
    EXPECT_EQ(*(--linehandler->end()), "kmp{m>2580:R,s>3306:A,x<420:R,R}");

    linehandler =
        std::make_unique<d19::FileLineHandler>(logger, input_file, '\n', 2);

    EXPECT_EQ(*linehandler->begin(), "rdg{a<2587:md,tgj}");
    EXPECT_EQ(*(--linehandler->end()), "qzd{m>2734:R,x>697:A,kmp}");
}

TEST_F(FixtureFileLineHandler, SingleLine) {

    linehandler =
        std::make_unique<d19::FileLineHandler>(logger, input_file, '\n', 3);

    EXPECT_EQ(*linehandler->begin(), "kmp{m>2580:R,s>3306:A,x<420:R,R}");
    EXPECT_EQ(*(--linehandler->end()), "kmp{m>2580:R,s>3306:A,x<420:R,R}");
}

TEST_F(FixtureFileLineHandler, Size) {

    linehandler =
        std::make_unique<d19::FileLineHandler>(logger, input_file, '\n', 0);

    EXPECT_EQ(linehandler->size(), 2);

    linehandler =
        std::make_unique<d19::FileLineHandler>(logger, input_file, '\n', 1);

    EXPECT_EQ(linehandler->size(), 2);

    linehandler =
        std::make_unique<d19::FileLineHandler>(logger, input_file, '\n', 2);

    EXPECT_EQ(linehandler->size(), 15);

    linehandler =
        std::make_unique<d19::FileLineHandler>(logger, input_file, '\n', 3);

    EXPECT_EQ(linehandler->size(), 1);

    linehandler =
        std::make_unique<d19::FileLineHandler>(logger, input_file, '\n', 4);

    EXPECT_EQ(linehandler->size(), 13);
}

TEST_F(FixtureFileLineHandler, LastBlockInFile) {

    linehandler =
        std::make_unique<d19::FileLineHandler>(logger, input_file, '\n', 4);

    EXPECT_EQ(*linehandler->begin(), "{x=342,m=311,a=770,s=1594}");
    EXPECT_EQ(*(--linehandler->end()), "{x=5,m=71,a=885,s=445}");
}

TEST_F(FixtureFileLineHandler, IterateEveryLine) {

    linehandler =
        std::make_unique<d19::FileLineHandler>(logger, input_file, '\n', 4);

    auto it = linehandler->begin();

    EXPECT_EQ(*it, "{x=342,m=311,a=770,s=1594}");
    EXPECT_EQ(*(++it), "{x=482,m=960,a=357,s=3234}");
    EXPECT_EQ(*(++it), "{x=129,m=8,a=1302,s=1683}");
    EXPECT_EQ(*(++it), "{x=2582,m=584,a=2405,s=2935}");
    EXPECT_EQ(*(++it), "{x=1139,m=3043,a=735,s=1128}");
    EXPECT_EQ(*(++it), "{x=1665,m=2277,a=272,s=915}");
    EXPECT_EQ(*(++it), "{x=472,m=2355,a=2508,s=285}");
    EXPECT_EQ(*(++it), "{x=137,m=571,a=1995,s=3237}");
    EXPECT_EQ(*(++it), "{x=2529,m=2131,a=459,s=587}");
    EXPECT_EQ(*(++it), "{x=403,m=111,a=1923,s=757}");
    EXPECT_EQ(*(++it), "{x=1539,m=278,a=2723,s=422}");
    EXPECT_EQ(*(++it), "{x=419,m=650,a=736,s=2491}");
    EXPECT_EQ(*(++it), "{x=5,m=71,a=885,s=445}");
}

TEST_F(FixtureFileLineHandler, SizeByIterator) {

    linehandler =
        std::make_unique<d19::FileLineHandler>(logger, input_file, '\n', 2);

    size_t count{};
    for (auto it = linehandler->begin(); it != linehandler->end(); ++it) {
        count += 1;
        EXPECT_TRUE((*it).size() > 0);
        //std::cout << *it << std::endl;
    }
    EXPECT_EQ(count, 15);

    linehandler =
        std::make_unique<d19::FileLineHandler>(logger, input_file, '\n', 4);

    count = 0;
    for (auto it = linehandler->begin(); it != linehandler->end(); ++it) {
        count += 1;
        EXPECT_TRUE((*it).size() > 0);
        if (count > 20) {
            assert(false);
        }
        //std::cout << *it << std::endl;
    }
    EXPECT_EQ(count, 13);
}

TEST_F(FixtureFileLineHandler, OtherDelimiter) {

    linehandler = std::make_unique<d19::FileLineHandler>(
        logger, input_file_delimiter, ',', 3);

    size_t count{};
    for (auto it = linehandler->begin(); it != linehandler->end(); ++it) {
        count += 1;
        EXPECT_TRUE((*it).size() > 0);
        //std::cout << *it << std::endl;
    }
    EXPECT_EQ(count, 4);
}
