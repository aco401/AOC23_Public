#ifndef NS_DAY_5_ILOG_H
#define NS_DAY_5_ILOG_H

#include <iostream>
#include <string>

namespace ns_day_5 {
class ILog {
    //  == INSTANTIATION ==
  protected:
    //  -- Constructors --
    ILog() = default;
    ILog(const ILog &) = default;
    ILog(ILog &&) = default;

  public:
    //  -- Destructors --
    virtual ~ILog(){}; // TODO undefined reference - needs to be implemented in derived classes

    //  == OPERATORS ==
  protected:
    //  -- Assignment --
    ILog &operator=(const ILog &) = default;
    ILog &operator=(ILog &&) = default;

    //  == METHODS ==
  public:
    virtual void log(const std::string &msg) = 0;
};
} // namespace ns_day_5

#endif // NS_DAY_5_ILOG_H