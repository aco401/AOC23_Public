#ifndef NS_DAY_5_CATEGORY_MAP_INPUT_FACTORY_H
#define NS_DAY_5_CATEGORY_MAP_INPUT_FACTORY_H

#include <ICategoryMapInputHandler.hpp>
#include <ILog.hpp>
#include <memory>
#include <optional>

namespace ns_day_5 {

namespace CategoryMapEnum {
enum class Type {
    SeedToSoil,
    SoilToFertilizer,
    FertilizerToWater,
    WaterToLight,
    LightToTemperature,
    TemperatureToHumidity,
    HumidityToLocation
};

static const Type All[] = { // To iterate over enum type
    Type::SeedToSoil,         Type::SoilToFertilizer,
    Type::FertilizerToWater,  Type::WaterToLight,
    Type::LightToTemperature, Type::TemperatureToHumidity,
    Type::HumidityToLocation};
}; // namespace CategoryMapEnum

/**
 * @brief Interface to 
 * 
 */
class CategoryMapInputFactory { // TODO look at Factory Pattern
  public:
    enum class ParseType { ConstArray = 0, FileParsing = 1 };
    CategoryMapInputFactory(std::shared_ptr<ns_day_5::ILog>);

    std::unique_ptr<ICategoryMapInputHandler>
    createInput(CategoryMapEnum::Type, ParseType,
                std::optional<std::string> parseParameter = std::nullopt);

  private:
    std::shared_ptr<ns_day_5::ILog> logger;
};

} // namespace ns_day_5

#endif // NS_DAY_5_CATEGORY_MAP_INPUT_FACTORY_H