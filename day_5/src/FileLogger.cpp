#include "FileLogger.hpp"
#include <cassert>
#include <iostream>

ns_day_5::FileLogger::FileLogger(const std::string &&str,
                                 std::ios_base::openmode mode)
    : fName{std::move(str)} {
    assert(!fName.empty());
    fStream = std::ofstream{fName, mode};
    assert(fStream.is_open());
};

void ns_day_5::FileLogger::log(const std::string &msg) { // Todo: Log Level?
    fStream << msg << "\n";
};