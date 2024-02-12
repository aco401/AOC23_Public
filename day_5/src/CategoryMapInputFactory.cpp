
#include <CategoryMapInputFactory.hpp>
#include <InputHandlerConstArray.hpp>
#include <InputHandlerTextfile.hpp>

ns_day_5::CategoryMapInputFactory::CategoryMapInputFactory(
    std::shared_ptr<ns_day_5::ILog> logger)
    : logger{logger} {};

std::unique_ptr<ns_day_5::ICategoryMapInputHandler>
ns_day_5::CategoryMapInputFactory::createInput(
    ns_day_5::CategoryMapEnum::Type mapping,
    ns_day_5::CategoryMapInputFactory::ParseType parsing,
    std::optional<std::string> parseParameter) {
    switch (parsing) {
    default:
    case ns_day_5::CategoryMapInputFactory::ParseType::ConstArray:
        return std::unique_ptr<ns_day_5::ICategoryMapInputHandler>(
            new ns_day_5::InputHandlerConstArray(logger, mapping));
        break;
    case ns_day_5::CategoryMapInputFactory::ParseType::FileParsing:
        return std::unique_ptr<ns_day_5::ICategoryMapInputHandler>(
            new ns_day_5::InputHandlerTextfile(logger, mapping,
                                               parseParameter.value_or("")));
    };
}
