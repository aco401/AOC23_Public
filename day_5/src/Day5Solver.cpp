#include <Day5Solver.hpp>
#include <DummyLogger.hpp>
#include <FileLogger.hpp>

#include <fstream>

#include <CategoryMapLocalStorage.hpp>

#include <array>
#include <limits>

#include <utility.hpp>

//#include <Data_Example.hpp>
std::vector<int> seeds{79, 14, 55, 13};

std::pair<int, int> ns_day_5::solve_file(std::shared_ptr<ns_day_5::ILog> logger,
                                         const std::string &filename) {
    const ns_day_5::CategoryMapInputFactory::ParseType parseType =
        ns_day_5::CategoryMapInputFactory::ParseType::FileParsing;

    // std::shared_ptr<ns_day_5::ILog> logger =
    //     std::make_shared<ns_day_5::FileLogger>(std::move(log_file));
    ns_day_5::CategoryMapInputFactory factory{logger};

    // Acquire all CategoryMaps (handlers)
    std::map<ns_day_5::CategoryMapEnum::Type,
             std::unique_ptr<ns_day_5::ICategoryMap>>
        maps{};
    for (auto mapping : ns_day_5::CategoryMapEnum::All) {
        maps.insert(
            {mapping, std::make_unique<ns_day_5::CategoryMapLocalStorage>(
                          logger, factory.createInput(
                                      mapping, parseType,
                                      std::optional<std::string>(filename)))});
    };

    // For each seed find location
    std::map<int, int> seedToLocation{};
    for (auto seed : seeds) {
        seedToLocation.insert(
            {seed, ns_day_5::findLocationForSeed(logger, seed, maps)});
    }
    // Find lowest location
    std::pair<int, int> lowest =
        ns_day_5::findLowestLocation(logger, seedToLocation);

    logger->log("Lowest Location is: " + std::to_string(lowest.second) +
                " for seed" + std::to_string(lowest.first));

    return lowest;
};

std::pair<int, int>
ns_day_5::solve_example(std::shared_ptr<ns_day_5::ILog> logger) {
    const ns_day_5::CategoryMapInputFactory::ParseType parseType =
        ns_day_5::CategoryMapInputFactory::ParseType::ConstArray;

    // std::shared_ptr<ns_day_5::ILog> logger =
    //     std::make_shared<ns_day_5::FileLogger>(std::move(log_file));
    ns_day_5::CategoryMapInputFactory factory{logger};

    // Acquire all CategoryMaps (handlers)
    std::map<ns_day_5::CategoryMapEnum::Type,
             std::unique_ptr<ns_day_5::ICategoryMap>>
        maps{};
    for (auto mapping : ns_day_5::CategoryMapEnum::All) {
        maps.insert(
            {mapping, std::make_unique<ns_day_5::CategoryMapLocalStorage>(
                          logger, factory.createInput(mapping, parseType))});
    };

    // For each seed find location
    std::map<int, int> seedToLocation{};
    for (auto seed : seeds) {
        seedToLocation.insert(
            {seed, ns_day_5::findLocationForSeed(logger, seed, maps)});
    }
    // Find lowest location
    std::pair<int, int> lowest =
        ns_day_5::findLowestLocation(logger, seedToLocation);

    logger->log("Lowest Location is: " + std::to_string(lowest.second) +
                " for seed" + std::to_string(lowest.first));

    return lowest;
};

// TODO function signature might only pass one seed at a time
int ns_day_5::findLocationForSeed(
    std::shared_ptr<ns_day_5::ILog> logger, const int seed,
    const std::map<ns_day_5::CategoryMapEnum::Type,
                   std::unique_ptr<ns_day_5::ICategoryMap>> &categories) {
    int location = seed;

    logger->log("findLocationForSeed: seed: " + std::to_string(seed));

    for (auto mapping : ns_day_5::CategoryMapEnum::All) {
        logger->log("findLocationForSeed: Prior Seed Mapping: " +
                    std::to_string(location));
        logger->log("findLocationForSeed: Mapping: " +
                    std::to_string(static_cast<int>(mapping)));
        logger->log(
            "findLocationForSeed: Seed: " + std::to_string(seed) +
            ", Found Category: " +
            std::to_string(categories.at(mapping)->findDestination(seed)));

        location = categories.at(mapping)->findDestination(
            location); // TODO at() vs find() vs operator[]#
        // TODO sd can be searched as this would always be seed
    }
    // logger->log(mapToString(seedToLocation));
    return location;
}

std::pair<int, int>
ns_day_5::findLowestLocation(std::shared_ptr<ns_day_5::ILog> logger,
                             const std::map<int, int> &m) {
    typename std::pair<int, int> lowest{std ::numeric_limits<int>::max(),
                                        std ::numeric_limits<int>::max()};
    for (auto [seed, location] : m) {
        if (location < lowest.second) {
            lowest = {seed, location};
        }
    }
    return lowest;
}