#ifndef NS_DAY_5_ICATEGORY_MAP_INPUT_HANDLER_H
#define NS_DAY_5_ICATEGORY_MAP_INPUT_HANDLER_H

#include <string>

namespace ns_day_5 {

/**
 * @brief Interface to 
 * 
 */
class ICategoryMapInputHandler {
  public:
    struct InputRow {
        long destination;
        long source;
        long length;

      public:
        std::string to_string() const {
            return "{" + std::to_string(destination) + ", " +
                   std::to_string(source) + ", " + std::to_string(length) + "}";
        };
    };

    //  == INSTANTIATION ==
  protected:
    //  -- Constructors --
    ICategoryMapInputHandler() = default;
    ICategoryMapInputHandler(const ICategoryMapInputHandler &) = default;
    ICategoryMapInputHandler(ICategoryMapInputHandler &&) = default;

  public:
    //  -- Destructors --
    virtual ~ICategoryMapInputHandler(){};

    //  == OPERATORS ==
  protected:
    //  -- Assignment --
    ICategoryMapInputHandler &
    operator=(const ICategoryMapInputHandler &) = default;
    ICategoryMapInputHandler &operator=(ICategoryMapInputHandler &&) = default;

    //  == METHODS ==
  public:
    enum class PtrMovementReturn { Successfull = 0, Failed = 1 };
    enum class PtrMovement { Next = 0, Prior = 1, Start = 2, End = 3 };

    virtual InputRow getRowAtPtr() = 0;
    virtual PtrMovementReturn movePtr(PtrMovement) = 0;
    virtual int getSizeRows() const = 0;
};

} // namespace ns_day_5

#endif // NS_DAY_5_ICATEGORY_MAP_INPUT_HANDLER_H