#include <../TraceException.hpp>
#include <FileLogger.hpp>
#include <cassert>
#include <iostream>

d19::FileLogger::FileLogger(const std::string &str,
                            std::ios_base::openmode mode)
    : fName{str} {

    if (str.empty()) {
        throw std::invalid_argument(
            throw_message("String Parameter str is empty!"));
    }

    fStream = std::ofstream{fName, mode};

    if (!fStream.good()) {
        throw std::ios_base::failure(
            throw_message("File stream to log is not in a correct state!"));
    }
};

void d19::FileLogger::log(const std::string &msg) { // Todo: Log Level?
    fStream << msg << "\n";
};

void d19::FileLogger::log(int integer) { // Todo: Log Level?
    fStream << std::to_string(integer) << "\n";
};

void d19::FileLogger::log(unsigned integer) { // Todo: Log Level?
    fStream << std::to_string(integer) << "\n";
};

void d19::FileLogger::log(size_t integer) { // Todo: Log Level?
    fStream << std::to_string(integer) << "\n";
};