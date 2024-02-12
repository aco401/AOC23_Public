#include <Day4Solver.hpp>
#include <FileLogger.hpp>
#include <ILogger.hpp>
#include <ScratchCardParser.hpp>

#include <ios>
#include <memory>
#include <regex>
#include <string>
#include <utility>

#include <fstream>

const constexpr char *inputFile = "src/input.txt";
const constexpr char *logFile = "src/log.txt";

const constexpr char *charLineRegex =
    "(.[^:])*:[ ]*[0-9]{1,2}([ ]+[0-9]{1,2})*[ ]*\\|[ ]*[0-9]{1,2}([ "
    "]+[0-9]{1,2})*[ ]*[\\n]*";

int ns_day4::solve() {
    std::shared_ptr<ns_day4::ILogger> logger =
        std::make_shared<ns_day4::FileLogger>(logFile);

    // open file from AdventOfCode // TODO IDEA: Use HTTPS Request to get Website Data
    std::basic_ifstream<char> stream{inputFile, std::ios_base::in};

    std::regex file_regex{charLineRegex, std::regex::basic};

    // Read scratchcard input
    ScratchCardParser parser{logger, stream, std::move(file_regex)};

    // Calculate points for every ScratchCard in the file
    int totalPoints = 0;

    while (auto status = parser.getNextCard()) {
        logger->log("TotalPoints: " + std::to_string(totalPoints));
        totalPoints += status.value().calculatePoints();
    }
    return totalPoints;
};