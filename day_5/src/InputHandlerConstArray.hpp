#ifndef NS_DAY_5_INPUT_HANDLER_CONST_ARRAY_H
#define NS_DAY_5_INPUT_HANDLER_CONST_ARRAY_H

#include <CategoryMapInputFactory.hpp>
#include <Data_Example.hpp>
#include <ICategoryMapInputHandler.hpp>
#include <ILog.hpp>
#include <array>
#include <memory>

namespace ns_day_5 {

/**
 * @brief Interface to 
 * 
 */
class InputHandlerConstArray : public ICategoryMapInputHandler {
  public:
    InputHandlerConstArray(std::shared_ptr<ns_day_5::ILog>,
                           ns_day_5::CategoryMapEnum::Type);

    InputRow getRowAtPtr() override;
    PtrMovementReturn movePtr(PtrMovement) override;
    int getSizeRows() const override;

  private:
    std::shared_ptr<ns_day_5::ILog> logger;
    ns_day_5::CategoryMapEnum::Type mapType;
    int data_index; // points to data allocated at compile time // TODO rule of 5 // use data_array
    int data_size;
};

}; // namespace ns_day_5

#endif // NS_DAY_5_INPUT_HANDLER_CONST_ARRAY_H