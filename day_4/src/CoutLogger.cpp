#include "CoutLogger.hpp"
#include <iostream>

#include <string>

void ns_day4::CoutLogger::log(const std::string &msg) {
    std::cout << msg << "\n";
};