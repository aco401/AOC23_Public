#include "FileLogger.hpp"
#include <cassert>
#include <iostream>

#include <fstream>
#include <string>

ns_day4::FileLogger::FileLogger(const std::string &&filename)
    : fName{filename} {
    assert(!fName.empty());
    fStream = std::ofstream{fName, std::ios::binary};
    assert(fStream.is_open());
};

void ns_day4::FileLogger::log(const std::string &msg) { // Todo: Log Level?
    fStream << msg << "\n";
};