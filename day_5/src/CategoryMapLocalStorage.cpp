#include <CategoryMapLocalStorage.hpp>

ns_day_5::CategoryMapLocalStorage::~CategoryMapLocalStorage() {
    ns_day_5::ICategoryMap::~ICategoryMap(); // no neccsarry
}

ns_day_5::CategoryMapLocalStorage::CategoryMapLocalStorage(
    std::shared_ptr<ILog> logger,
    std::unique_ptr<ICategoryMapInputHandler> handler)
    : logger{logger}, handler{std::move(handler)},
      data{} {
          //logger->log(handler->getRowAtPtr());
      };

enum class findDestinationReturn { NotFound = -1 };

int ns_day_5::CategoryMapLocalStorage::findDestination(
    int source) const { // Return should be std:: optional
    // search for destinations in map the parameter source is inbetween

    logger->log("findDestination: source: " + std::to_string(source));

    int destination = static_cast<int>(findDestinationReturn::NotFound);
    handler->movePtr(ns_day_5::ICategoryMapInputHandler::PtrMovement::Start);
    for (int i = 0; i < handler->getSizeRows(); i++) {
        ns_day_5::ICategoryMapInputHandler::InputRow curr_row =
            handler->getRowAtPtr();
        logger->log("findDestination: Iteration: " + curr_row.to_string());
        if ((source >= curr_row.source) &&
            (source <= curr_row.source + curr_row.length)) {
            int diff = source - curr_row.source;
            logger->log("findDestination: Diff: " + std::to_string(diff) +
                        "source: " + std::to_string(source) +
                        "curr_row.source: " + std::to_string(curr_row.source));
            destination = curr_row.destination + diff;
            break;
        }
        handler->movePtr(ns_day_5::ICategoryMapInputHandler::PtrMovement::Next);
    }
    if (destination == static_cast<int>(findDestinationReturn::NotFound)) {
        destination = source;
    }

    return destination;
};
