#include <iostream>

#include "Day4Solver.hpp"

#include <string>

int main(int /*unused*/, char ** /*unused*/) {

    std::cout << "Total pile of ScratchCards is worth " +
                     std::to_string(ns_day4::solve())
              << "\n";
    return 0;
}
