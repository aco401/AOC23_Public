#ifndef AOC_D19_ILOG_H
#define AOC_D19_ILOG_H

#include <iostream>
#include <string>

namespace d19 {
class ILog {
    //  == INSTANTIATION ==
  protected:
    //  -- Constructors --
    ILog() = default;
    ILog(const ILog &) = default;
    ILog(ILog &&) = default;

  public:
    //  -- Destructors --
    virtual ~ILog() = default;

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
} // namespace d19

#endif // AOC_D19_ILOG_H