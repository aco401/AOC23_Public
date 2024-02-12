#ifndef NS_DAY_5_ILOG_H
#define NS_DAY_5_ILOG_H

#include <iostream>
#include <string>

namespace d17 {
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
    virtual void log(int integer) = 0;
    virtual void log(unsigned integer) = 0;
    virtual void log(size_t integer) = 0;
};
} // namespace d17

#endif // NS_DAY_5_ILOG_H