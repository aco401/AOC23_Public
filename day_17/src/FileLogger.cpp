#include "FileLogger.hpp"
#include <cassert>
#include <iostream>

d17::FileLogger::FileLogger(const std::string &&str,
                            std::ios_base::openmode mode)
    : fName{std::move(str)} {
    assert(!fName.empty());
    fStream = std::ofstream{fName, mode};
    assert(fStream.is_open());
};

void d17::FileLogger::log(const std::string &msg) { // Todo: Log Level?
    fStream << msg << "\n";
};

void d17::FileLogger::log(int integer) { // Todo: Log Level?
    fStream << std::to_string(integer) << "\n";
};

void d17::FileLogger::log(unsigned integer) { // Todo: Log Level?
    fStream << std::to_string(integer) << "\n";
};

void d17::FileLogger::log(size_t integer) { // Todo: Log Level?
    fStream << std::to_string(integer) << "\n";
};