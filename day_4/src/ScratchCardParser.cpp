#include "ScratchCardParser.hpp"
#include "utility.hpp"
#include <cassert>
#include <vector>

ScratchCardParser::ScratchCardParser(std::shared_ptr<ns_day4::ILogger> logger,
                                     std::basic_istream<char> &stream,
                                     std::regex &&regex)
    : logger{logger}, parserStream{stream}, parserRegex{std::move(regex)} {
    assert(stream.good());
    // invariant

    assert(!stream.eof());
    // precondition - might be true after getting all ScratchCard
};

/**
 * @details Design decision:
 *  - Pass container of int values to ScratchCard. Goal: be efficient in time
 *      1. Pass container to ScratchCard constructor and return ScratchCard [out - pass by value]
 *          - might be inefficient -> involve a lot of copying (TODO inform about move/copy constructor)
 *          --> solution view ScratchCardParser as data source and use move semantics to avoid copying!
 *      2. Create Scratch with empty containers, use reference to fill ScratchCard [in/out] pass-by-reference
 *          2.1 Give access to internal container to append to
 *              - breaks encapsulation
 *          2.2 Use ScratchCard interface to fill container by value
 *              + embraces encapsulation
 *              - might result in unecessary function calls
 */
std::optional<ScratchCard> ScratchCardParser::getNextCard() {

    // Get the next text line
    std::string line{};
    std::getline(parserStream, line);

    logger->log(line);

    if (!line.empty()) {
        // Check precondition of class
        assert(std::regex_match(line,
                                parserRegex)); // Assert with regex

        // TODO Substr with RegEx

        // Separate winning numbers and OwningNumbers
        // Find the ":" character
        int characterPos = line.find(":");

        // Remove "Card x :" section
        line = line.substr(characterPos + 1);

        // Find the "|" character
        characterPos = line.find("|");

        // Create vectors
        std::vector<int> winningNums{};
        std::vector<int> owningNums{};

        // Parse strings to vector of int values
        ns_day4::getVectorFromString(line.substr(0, characterPos - 1),
                                     winningNums);
        ns_day4::getVectorFromString(line.substr(characterPos + 1), owningNums);
        return std::optional<ScratchCard>(
            ScratchCard(logger, std::move(winningNums), std::move(owningNums)));
    }
    logger->log("Line Empty");
    return std::optional<ScratchCard>(std::nullopt);
}
