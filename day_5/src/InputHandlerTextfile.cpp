#include <InputHandlerTextfile.hpp>
#include <cassert>
#include <regex>

const std::string regexOnlyNumbers{"[0-9]+"};

// int mappingToSize(ns_day_5::CategoryMapEnum::Type mapping);
std::string mappingToString(ns_day_5::CategoryMapEnum::Type mapping);

int const rowSize = 3; // TODO might be parametrized

ns_day_5::InputHandlerTextfile::InputHandlerTextfile(
    std::shared_ptr<ns_day_5::ILog> logger,
    ns_day_5::CategoryMapEnum::Type mapping, const std::string &filename)
    : logger{logger}, mapType{mapping}, data_index{0}, data_size{0},
      input_stream{filename.c_str(), std::ios_base::in} {
    logger->log("InputHandlerTextfile for " + mappingToString(mapping) +
                " created");

    // 1. Open a input_stream for the file - check openness
    assert(input_stream.is_open()); // TODO prior good()

    // 2. Calculate size of searched vector in textfile
    data_size = elementsOfMapping();
};

ns_day_5::ICategoryMapInputHandler::InputRow
ns_day_5::InputHandlerTextfile::getRowAtPtr() {
    return ns_day_5::ICategoryMapInputHandler::InputRow{
        getValueOfIndex(data_index).value_or(0),
        getValueOfIndex(data_index + 1).value_or(0),
        getValueOfIndex(data_index + 2)
            .value_or(0)}; //TODO might better throw an exception
};

ns_day_5::ICategoryMapInputHandler::PtrMovementReturn
ns_day_5::InputHandlerTextfile::movePtr(
    ns_day_5::ICategoryMapInputHandler::PtrMovement movement) {
    ns_day_5::ICategoryMapInputHandler::PtrMovementReturn ret =
        ns_day_5::ICategoryMapInputHandler::PtrMovementReturn::Successfull;
    switch (movement) {
    case PtrMovement::Next:
        if ((data_index + rowSize) < data_size) {
            data_index += rowSize;
        } else {
            ret = ns_day_5::ICategoryMapInputHandler::PtrMovementReturn::Failed;
        }
        break;
    case PtrMovement::Prior:
        if ((data_index - rowSize) >= 0) {
            data_index += rowSize;
        } else {
            ret = ns_day_5::ICategoryMapInputHandler::PtrMovementReturn::Failed;
        }
        break;
    case PtrMovement::Start:
        data_index = 0;
        break;
    case PtrMovement::End:
        data_index = data_size - rowSize;
        break;
    }

    return ret;
};

int ns_day_5::InputHandlerTextfile::getSizeRows() const {
    return data_size / rowSize;
};

/**
 * @brief Maps the Enum ns_day_5::CategoryMapEnum::Type to a String representation used in textfiles.
 * 
 * @param mapping searched mapping
 * @return std::string name of vector in textfile
 */
std::string mappingToString(ns_day_5::CategoryMapEnum::Type mapping) {
    using namespace ns_day_5;
    switch (mapping) {
    case CategoryMapEnum::Type::SeedToSoil:
        return "seed-to-soil";
        break;
    case CategoryMapEnum::Type::SoilToFertilizer:
        return "soil-to-fertilizer";
        break;
    case CategoryMapEnum::Type::FertilizerToWater:
        return "fertilizer-to-water";
        break;
    case CategoryMapEnum::Type::WaterToLight:
        return "water-to-light";
        break;
    case CategoryMapEnum::Type::LightToTemperature:
        return "light-to-temperature";
        break;
    case CategoryMapEnum::Type::TemperatureToHumidity:
        return "temperature-to-humidity";
        break;
    case CategoryMapEnum::Type::HumidityToLocation:
        return "humidity-to-location";
        break;
    default:
        return "";
    }
}

std::optional<int> findMapPos(std::shared_ptr<ns_day_5::ILog> logger,
                              std::ifstream &input_stream,
                              ns_day_5::CategoryMapEnum::Type mapType) {

    // TODO might be optimized by putting ptr to start of map
    input_stream.seekg(0);

    // Get the next text line
    std::string line{};
    std::getline(input_stream, line);

    logger->log("findMapPos: Line: " + line);

    // Find line with map name
    while (!input_stream.eof()) {
        // Find the " map:"
        int mapNameEnd = line.find(" map:");
        if (mapNameEnd != std::string::npos) {
            std::string localMapName = line.substr(0, mapNameEnd);

            // Compare with given name of searched vector
            if (localMapName.compare(mappingToString(mapType)) == 0) {
                return input_stream.tellg();
                break;
            }
        }

        // Increment line to look at
        std::getline(input_stream, line);
    }

    return std::nullopt;
}

int ns_day_5::InputHandlerTextfile::elementsOfMapping() {
    using namespace ns_day_5;
    int numberOfElements{};

    std::regex reg{regexOnlyNumbers};

    // 1. Find line with map name
    std::optional<int> mapStart =
        findMapPos(logger, input_stream, mapType).value();

    // 2. Count all numbers words
    if (mapStart != std::nullopt) {
        std::string line{};
        input_stream.seekg(mapStart.value());
        std::getline(input_stream, line);

        while (!line.empty() && !input_stream.eof()) {

            //
            size_t posStartWord = 0;
            size_t posEndWord = line.find(" ", posStartWord);

            // TODO might miss numbers without any " " characters
            while (posEndWord != std::string::npos) {
                std::string localNumber =
                    line.substr(posStartWord, posEndWord - posStartWord);

                // check if word only has numbers
                if (std::regex_match(localNumber, reg)) {

                    numberOfElements += 1;
                }

                // Increment Word selection
                posStartWord = posEndWord + 1;
                posEndWord = line.find(" ", posStartWord);

                if ((posEndWord == std::string::npos) &&
                    posStartWord <= (line.length() - 1)) {
                    posEndWord = line.length();
                }
            }

            // Increment line to look at
            std::getline(input_stream, line);
        }
    }

    return numberOfElements;
};

std::optional<long> ns_day_5::InputHandlerTextfile::getValueOfIndex(int index) {

    logger->log("getValueOfIndex: index" + std::to_string(index));

    if (index >= data_size) {
        return std::nullopt; // TODO commenting in function description
    }

    int indexOfCurrElement{};

    std::regex reg{regexOnlyNumbers};

    // 1. Find line with map name
    std::optional<int> mapStart = findMapPos(logger, input_stream, mapType);

    // 2. Find number of Index
    if (mapStart !=
        std::nullopt) { // TODO duplication needs to be solved !!!!!!!!!!
        std::string line{};
        input_stream.seekg(mapStart.value());
        std::getline(input_stream, line);

        while (!line.empty() && !input_stream.eof()) {

            //
            size_t posStartWord = 0;
            size_t posEndWord = line.find(" ", posStartWord);

            // TODO might miss numbers without any " " characters
            while (posEndWord != std::string::npos) {
                std::string localNumber =
                    line.substr(posStartWord, posEndWord - posStartWord);
                logger->log("localNumber: " + localNumber);
                // check if word only has numbers
                if (std::regex_match(localNumber, reg)) {
                    logger->log("localNumber: " + localNumber);
                    if (indexOfCurrElement == index) {
                        logger->log("getValueOfIndex: index corresponds to " +
                                    localNumber);
                        return std::optional<long>(std::stol(
                            localNumber)); // TODO problem int is too small for temp use long
                    }
                    indexOfCurrElement += 1;
                }

                // Increment Word selection
                posStartWord = posEndWord + 1;
                posEndWord = line.find(" ", posStartWord);
                logger->log("posStartWord: " + std::to_string(posStartWord));
                logger->log("posEndWord: " + std::to_string(posEndWord));
                if ((posEndWord == std::string::npos) &&
                    posStartWord <= (line.length() - 1)) {
                    posEndWord = line.length();
                }
                logger->log("posEndWord: " + std::to_string(posEndWord));
            }

            // Increment line to look at
            std::getline(input_stream, line);
        }
    }

    return std::nullopt;
}
