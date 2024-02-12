#include <Data_Example.hpp>
#include <InputHandlerConstArray.hpp>

//#define SIZEOF(arr) ((sizeof(arr)) / (sizeof(arr[0])))
#define SIZEOF(arr) (arr.size())

const int *mappingToArray(ns_day_5::CategoryMapEnum::Type mapping);
int mappingToSize(ns_day_5::CategoryMapEnum::Type mapping);

int const rowSize = 3;

ns_day_5::InputHandlerConstArray::InputHandlerConstArray(
    std::shared_ptr<ns_day_5::ILog> logger,
    ns_day_5::CategoryMapEnum::Type mapping)
    : logger{logger}, mapType{mapping}, data_index{0}, data_size{mappingToSize(
                                                           mapping)} {
    logger->log(std::to_string(static_cast<int>(mapping)));
};

ns_day_5::ICategoryMapInputHandler::InputRow
ns_day_5::InputHandlerConstArray::getRowAtPtr() {
    return ns_day_5::ICategoryMapInputHandler::InputRow{
        mappingToArray(mapType)[data_index],
        mappingToArray(mapType)[data_index + 1],
        mappingToArray(mapType)[data_index + 2]};
};

ns_day_5::ICategoryMapInputHandler::PtrMovementReturn
ns_day_5::InputHandlerConstArray::movePtr(
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
int ns_day_5::InputHandlerConstArray::getSizeRows() const {
    return data_size / rowSize;
};

// TODO might be moved to factory - SOC
const int *mappingToArray(ns_day_5::CategoryMapEnum::Type mapping) {
    using namespace ns_day_5;
    switch (mapping) {
    case CategoryMapEnum::Type::SeedToSoil:
        return data_example::seedToSoil.begin();
        break;
    case CategoryMapEnum::Type::SoilToFertilizer:
        return data_example::soilToFertilizer.begin();
        break;
    case CategoryMapEnum::Type::FertilizerToWater:
        return data_example::fertilizerToWater.begin();
        break;
    case CategoryMapEnum::Type::WaterToLight:
        return data_example::waterToLight.begin();
        break;
    case CategoryMapEnum::Type::LightToTemperature:
        return data_example::lightToTemperature.begin();
        break;
    case CategoryMapEnum::Type::TemperatureToHumidity:
        return data_example::temperatureToHumidity.begin();
        break;
    case CategoryMapEnum::Type::HumidityToLocation:
        return data_example::HumidityToLocation.begin();
        break;
    default:
        return nullptr;
    }
};

int mappingToSize(ns_day_5::CategoryMapEnum::Type mapping) {
    using namespace ns_day_5;
    switch (mapping) {
    case CategoryMapEnum::Type::SeedToSoil:
        return SIZEOF(data_example::seedToSoil);
        break;
    case CategoryMapEnum::Type::SoilToFertilizer:
        return SIZEOF(data_example::soilToFertilizer);
        break;
    case CategoryMapEnum::Type::FertilizerToWater:
        return SIZEOF(data_example::fertilizerToWater);
        break;
    case CategoryMapEnum::Type::WaterToLight:
        return SIZEOF(data_example::waterToLight);
        break;
    case CategoryMapEnum::Type::LightToTemperature:
        return SIZEOF(data_example::lightToTemperature);
        break;
    case CategoryMapEnum::Type::TemperatureToHumidity:
        return SIZEOF(data_example::temperatureToHumidity);
        break;
    case CategoryMapEnum::Type::HumidityToLocation:
        return SIZEOF(data_example::HumidityToLocation);
        break;
    default:
        return 0;
    }
};
