#ifndef INPUT_HANDLER_TEXTFILE_H
#define INPUT_HANDLER_TEXTFILE_H

#include <CategoryMapInputFactory.hpp>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <fstream>
#include <iostream>

#include <optional>

namespace ns_day_5 {
class InputHandlerTextfile : public ICategoryMapInputHandler {
  public:
    InputHandlerTextfile(std::shared_ptr<ILog> logger,
                         ns_day_5::CategoryMapEnum::Type mapping,
                         const std::string &filename);

    InputRow getRowAtPtr() override;
    PtrMovementReturn movePtr(PtrMovement) override;
    int getSizeRows() const override;

    // Might be moved to another class - SRP
    int elementsOfMapping();
    std::optional<long> getValueOfIndex(int index);

  private:
    std::shared_ptr<ns_day_5::ILog> logger;
    ns_day_5::CategoryMapEnum::Type mapType;
    int data_index; // points to data allocated at compile time // TODO rule of 5 // use data_array
    int data_size;
    std::ifstream input_stream;
};
} // namespace ns_day_5

#endif // INPUT_HANDLER_TEXTFILE_H