#include <InputHandlerTextfile.hpp>
#include <cassert>
#include <regex>

ns_day_5::TextfileArray::TextfileArray(std::shared_ptr<ILog> logger,
                                       const std::string &filename,
                                       const std::string &vectorName)
    : logger{logger}, vectorName{vectorName}, data_index{0}, data_size{0},
      input_stream{filename.c_str(), std::ios_base::in} {
    logger->log("TextfileArray for " + mappingToString(mapping) + " created");

    // 1. Open a input_stream for the file - check openness
    assert(input_stream.is_open()); // TODO prior good()

    // 2. Calculate size of searched vector in textfile
    data_size = elementsOfMapping();
};

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
