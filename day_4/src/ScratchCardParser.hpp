#ifndef SCRATCHCARD_PARSER_H
#define SCRATCHCARD_PARSER_H

#include "ILogger.hpp"
#include "ScratchCard.hpp"
#include <iostream>
#include <memory>
#include <optional>
#include <regex>
#include <string>

/**
 * A handle for parsing stream of chars containing ScratchCards
 */
class ScratchCardParser {
  public:
    /**
     * @brief Construct a new Scratch Card Parser object. To parse ScratchCards from a char stream
     * 
     * The character lines should look like:
     * [any character] ":" [1 or 2 digit number separated by " "] "|" [1 or 2 digit number separated by " "] \n
     * 
     * In terms of RegEx: 
     * (.[^:])*:([ ]+[0-9]{1,2})+[ ]+\|([ ]+[0-9]{1,2})+
     * 
     * Example line:
     * Card   1: 69 12 75 19 83 56 73 53 52 91 | 83 63 56 30 77 94 12 81 42 69 52 70  6 97 20 43 61 22 75 19 73 32 74 53 91
     * 
     * @param logger a way to document and debug
     * @param stream [stream.good() && !stream.eof] stream to parse the ScratchCard from
     * 
     */
    ScratchCardParser(std::shared_ptr<ns_day4::ILogger> logger,
                      std::basic_istream<char> &stream, std::regex &&regex);

    /**
     * @brief Get the Next Card object by reading the next line from 
     *        the char stream until the end-of-file has been reached
     *    
     * 
     * @return std::optional<ScratchCard> return Scratchcard until end-of-file reached
     */
    std::optional<ScratchCard> getNextCard();

  private:
    std::shared_ptr<ns_day4::ILogger>
        logger;                             // shared ptr as ownership is shared
    std::basic_istream<char> &parserStream; // TODO unique_ref ?
    std::regex parserRegex;
    // TODO no const or reference data members in a copyable or movable type ISOCPP C.12
};

#endif // SCRATCHCARD_PARSER_H