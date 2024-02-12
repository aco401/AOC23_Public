#include "utility.hpp"
#include <cassert>

void ns_day4::getVectorFromString(const std::string &in,
                                  std::vector<int> &out) {
    assert(out.empty()); // precondition
    assert(!in.empty()); // precondition

    std::string tmpAccumulator{};
    bool numberSeen = false;

    for (auto elem : in) {
        if (std::isdigit(elem)) {
            tmpAccumulator += elem;
            numberSeen = true;
        } else if (numberSeen) {
            numberSeen = false;
            out.push_back(std::stoi(tmpAccumulator));
            tmpAccumulator.clear();
        }
    }

    // if string ends with number and no whitespace
    if (!tmpAccumulator.empty()) {
        out.push_back(std::stoi(tmpAccumulator));
    }
}