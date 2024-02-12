#include "ScratchCard.hpp"
#include <algorithm>
#include <cassert>

ScratchCard::ScratchCard(std::shared_ptr<ns_day4::ILogger> logger,
                         std::vector<int> &&winningsNums,
                         std::vector<int> &&owningNums)
    : logger{logger}, winningsNumbers{winningsNums}, owningNumbers{owningNums} {
    assert(winningsNumbers.size() > 0); // precondition
    assert(owningNumbers.size() > 0);   // precondition
};

int ScratchCard::calculatePoints() const {
    int points = 0;
    for (int w : winningsNumbers) {

        auto found = std::find(owningNumbers.begin(), owningNumbers.end(), w);

        if (found != owningNumbers.end()) {
            if (points == 0) {
                points = 1;
            } else {
                points *= 2;
            }
        }
    }
    logger->log(std::to_string(points));

    assert(points >= 0); // postcondition
    return points;
};
