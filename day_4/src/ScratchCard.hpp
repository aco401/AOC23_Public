#ifndef SCRATCHCARD_H
#define SCRATCHCARD_H
#include "ILogger.hpp"
#include <memory>
#include <vector>

class ScratchCard {

  public:
    /**
     * @brief Construct a new Scratch Card:: Scratch Card object
     * 
     * @param logger used for documenting and debugging
     * @param winningsNums [elements > 0] numbers that you can potentially win with
     * @param owningNums [elements > 0] numbers that you actually own
     */
    ScratchCard(std::shared_ptr<ns_day4::ILogger> logger,
                std::vector<int> &&winningsNums, std::vector<int> &&owningNums);

    /**
     * @brief Count how many of the owning numbers are included in list of winning numbers.
     * The first match adds 1 point.
     * For every further match the points are doubled.
     * 
     * @return int [>=0] number of matching points
     */
    int calculatePoints() const;
    // BB: member function that does not (and will not ever) modify the state should be made const

  private:
    std::shared_ptr<ns_day4::ILogger>
        logger; // TODO check dependency injection variants
    std::vector<int> winningsNumbers;
    std::vector<int> owningNumbers;
};

#endif // SCRATCHCARD_H