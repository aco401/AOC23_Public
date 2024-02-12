#include <DummyLogger.hpp>
#include <ScratchCardParser.hpp>
#include <fstream>
#include <gtest/gtest.h>

using namespace ns_day4;

/* TODO: 
    - Add wrong ScratchCardFormat */

const std::string filename_one_card =
    "AdventOfCode2023/day_4/"
    "tests/input_one_card.txt";

const std::string filename_full =
    "AdventOfCode2023/day_4/"
    "src/input.txt";

const std::string charLineRegex(
    "(.[^:])*:[ ]*[0-9]{1,2}([ ]+[0-9]{1,2})*[ ]*\\|[ ]*[0-9]{1,2}([ "
    "]+[0-9]{1,2})*[ ]*[\\n]*");

/* // Demonstrate some basic assertions.
TEST(all_test, FileOpening) {
    //Expect an error as no valid file name is given
    EXPECT_ANY_THROW(ScratchCardParser(DummyLogger(), ""));

    // Correct Filename given
    EXPECT_NO_THROW(ScratchCardParser(DummyLogger(), filename_one_card));
} */

TEST(all_test, CardParsing) {
    std::basic_ifstream<char> stream{filename_one_card.c_str(),
                                     std::ios_base::in};
    std::shared_ptr<ns_day4::ILogger> logger =
        std::make_shared<ns_day4::DummyLogger>();

    std::regex file_regex{charLineRegex};

    ScratchCardParser parser{
        logger, stream,
        std::move(file_regex)}; // TODO moving a file global constant?

    // Should find a Card
    EXPECT_EQ(parser.getNextCard().has_value(), true);

    // Should not find a Card
    EXPECT_EQ(parser.getNextCard().has_value(), false);
}

TEST(all_test, CardParsingAndValueChecking) {
    std::basic_ifstream<char> stream{filename_one_card.c_str(),
                                     std::ios_base::in};
    std::shared_ptr<ns_day4::ILogger> logger =
        std::make_shared<ns_day4::DummyLogger>();

    std::regex file_regex{charLineRegex};

    ScratchCardParser parser{logger, stream, std::move(file_regex)};

    // Should find a Card and calculatePoints
    EXPECT_EQ(parser.getNextCard().value().calculatePoints(), 512);
}

TEST(all_test, MultipleCardParsing) {
    std::basic_ifstream<char> stream{filename_full.c_str(), std::ios_base::in};
    std::shared_ptr<ns_day4::ILogger> logger =
        std::make_shared<ns_day4::DummyLogger>();

    std::regex file_regex{charLineRegex};

    ScratchCardParser parser{logger, stream, std::move(file_regex)};
    EXPECT_EQ(parser.getNextCard().has_value(), true);
    EXPECT_EQ(parser.getNextCard().has_value(), true);
    EXPECT_EQ(parser.getNextCard().has_value(), true);
    EXPECT_EQ(parser.getNextCard().has_value(), true);
    EXPECT_EQ(parser.getNextCard().has_value(), true);
}

TEST(all_test, RegEXForInputTxt) { // Not UnitTesting

    const std::regex regex_start(
        "(.[^:])*:"); // (.[^:])*:([ ]+[0-9]{1,2})+[ ]+|([ ]+[0-9]{1,2})+
    std::string card_start{"Card 212:"};
    EXPECT_EQ(std::regex_match(card_start, regex_start), true);

    const std::regex regex_num(
        "[ ]*[0-9]{1,2}([ ]+[0-9]{1,2})*[ ]*"); // (.[^:])*:([ ]+[0-9]{1,2})+[ ]+|([ ]+[0-9]{1,2})+
    std::string card_num{" 98 19 95 93 62 76 36 70  1 55 "};
    EXPECT_EQ(std::regex_match(card_num, regex_num), true);

    const std::regex regex_num_sep(
        "[ ]*[0-9]{1,2}([ ]+[0-9]{1,2})*[ ]*\\|[ ]*[0-9]{1,2}([ "
        "]+[0-9]{1,2})*[ ]*"); // (.[^:])*:([ ]+[0-9]{1,2})+[ ]+|([ ]+[0-9]{1,2})+
    std::string card_num_sep{
        "98 19 95 93 62 76 36 70  1 55 |  5 22 21 58 75 85 31 67 49 "
        "73 47 90 34 24 99 37 50 11 23 48 96 87  6 29 91"};
    EXPECT_EQ(std::regex_match(card_num_sep, regex_num_sep), true);

    const std::regex regex_num_sep_nl(
        "[ ]*[0-9]{1,2}([ ]+[0-9]{1,2})*[ ]*\\|[ ]*[0-9]{1,2}([ "
        "]+[0-9]{1,2})*[ ]*[\\n]*"); // (.[^:])*:([ ]+[0-9]{1,2})+[ ]+|([ ]+[0-9]{1,2})+
    std::string card_num_sep_nl{
        "98 19 95 93 62 76 36 70  1 55 |  5 22 21 58 75 85 31 67 49 "
        "73 47 90 34 24 99 37 50 11 23 48 96 87  6 29 91\n"};
    EXPECT_EQ(std::regex_match(card_num_sep_nl, regex_num_sep_nl), true);

    std::string card_full{
        "Card 212: 98 19 95 93 62 76 36 70  1 55 |  5 22 21 58 75 85 31 67 49 "
        "73 47 90 34 24 99 37 50 11 23 48 96 87  6 29 91"};

    std::regex file_regex{charLineRegex};

    EXPECT_EQ(std::regex_match(card_full, file_regex), true);

    std::string card_full_2{
        "Card  20:  7  4 51  9 16 56  3 81 85 21 | 31 78 15 17 35 55 83 33 48 "
        "12 60 77 92 65 30 72 43 39 64 62 63  1 14 58  5\n"};

    EXPECT_EQ(std::regex_match(card_full_2, file_regex), true);
}
