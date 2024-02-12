#include "utility.hpp"
#include <cassert>

void ns_day_5::getVectorFromString(const std::string &in,
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

/**
 * @brief 
 * 
 * @param m 
 * @return std::string 
 */
std::string ns_day_5::mapToString(const std::map<int, int> &m) {
    // TODO mapToString
    std::string ret{""};
    if (m.size() > 0) {
        ret += "{";
        for (auto [key, value] : m) {
            ret +=
                "{" + std::to_string(key) + ", " + std::to_string(value) + "}";
        }
        ret += "}";
    }
    return ret;
};