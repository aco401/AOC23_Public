#ifndef ADVENTOFCODE2023_DAY_4_SRC_ILOGGER_HPP
#define ADVENTOFCODE2023_DAY_4_SRC_ILOGGER_HPP

#include <iostream>
#include <string>

namespace ns_day4 {
class ILogger {

    //  == INSTANTIATION ==
  protected:
    //  -- Constructors --
    ILogger() = default;
    ILogger(const ILogger &) = default;
    ILogger(ILogger &&) = default;

  public:
    //  -- Destructors --
    virtual ~ILogger() = 0;

    //  == OPERATORS ==
  protected:
    //  -- Assignment --
    ILogger &operator=(const ILogger &) = default;
    ILogger &operator=(ILogger &&) = default;

    //  == METHODS ==
  public:
    virtual void log(const std::string &msg) = 0;
};
} // namespace ns_day4

#endif // ADVENTOFCODE2023_DAY_4_SRC_ILOGGER_HPP