#include "utility.hpp"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <list>
#include <map>
#include <string>
#include <vector>

void d19::getVectorFromString(const std::string &input,
                              std::vector<unsigned> &out) {
    assert(out.empty());    // precondition
    assert(!input.empty()); // precondition

    std::string tmpAccumulator{};
    bool numberSeen = false;

    for (auto elem : input) {
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
std::string d19::mapToString(const std::map<unsigned, unsigned> &map) {
    std::string ret{""};
    if (!map.empty()) {
        ret += "{";
        for (auto [key, value] : map) {
            ret +=
                "{" + std::to_string(key) + ", " + std::to_string(value) + "}";
        }
        ret += "}";
    }
    return ret;
};

std::string d19::vectorToString(const std::vector<unsigned> &vec) {

    std::string ret{""};
    if (!vec.empty()) {
        ret += "{";
        for (auto value : vec) {
            ret += std::to_string(value) + ", ";
        }
        ret += "}";
    }
    return ret;
};

std::string d19::listToString(const std::list<unsigned> &vec) {

    std::string ret{""};
    if (!vec.empty()) {
        ret += "{";
        for (auto value : vec) {
            ret += std::to_string(value) + ", ";
        }
        ret += "}";
    }
    return ret;
};

unsigned d19::findUnusedKey(std::vector<unsigned> &keys) {

    if (keys.empty()) {
        return 0;
    }

    auto const predicate = [](int a, int b) { return a + 1 != b; };
    std::sort(keys.begin(), keys.end());

    if (keys.front() != 0) {
        return 0;
    } // first element was missing

    auto it = std::adjacent_find(keys.begin(), keys.end(), predicate);
    if (it == keys.end()) {
        --it;
    } // last element was missing
    return *it + 1;
}
