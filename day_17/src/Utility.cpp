#include "utility.hpp"

#include <cassert>

void d17::getVectorFromString(const std::string &in,
                              std::vector<unsigned> &out) {
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
std::string d17::mapToString(const std::map<unsigned, unsigned> &m) {
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

std::string d17::vectorToString(const std::vector<unsigned> &v) {

    std::string ret{""};
    if (v.size() > 0) {
        ret += "{";
        for (auto value : v) {
            ret += std::to_string(value) + ", ";
        }
        ret += "}";
    }
    return ret;
};

std::string d17::listToString(const std::list<unsigned> &v) {

    std::string ret{""};
    if (v.size() > 0) {
        ret += "{";
        for (auto value : v) {
            ret += std::to_string(value) + ", ";
        }
        ret += "}";
    }
    return ret;
};

std::string
d17::vectorToString(const std::vector<d17::IGraph::NeighbourDirection> &v) {
    std::string ret{""};
    if (v.size() > 0) {
        ret += "{";
        for (auto value : v) {
            ret += std::to_string(static_cast<unsigned>(value)) + ", ";
        }
        ret += "}";
    }
    return ret;
};

std::string
d17::queueTopToString(const std::priority_queue<d17::IGraph::Node> &queue) {

    return queue.top().to_string();
    // std::string ret{""};
    // if (v.size() > 0) {
    //     ret += "{";
    //     for (auto value : v) {
    //         ret += std::to_string(static_cast<unsigned>(value)) + ", ";
    //     }
    //     ret += "}";
    // }
    // Not Implemnted
}