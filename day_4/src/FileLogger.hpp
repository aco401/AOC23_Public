#ifndef NS_DAY4_FILE_LOGGER_H
#define NS_DAY4_FILE_LOGGER_H

#include "ILogger.hpp"
#include <fstream>
#include <iostream>
#include <string>

namespace ns_day4 { // TODO: check best practices of "using namespace ..."
class FileLogger : public ILogger {
  public:
    /**
     * @brief Construct a new Local Logger:: Local Logger object
     *
     * @param filename [in] message to log
     *
     * @details Parameter passing: str[in] use f(const X&) to comply with
     * ISOCPP.F15
     */
    explicit FileLogger(const std::string &&filename); // explicit -> ISOCPP:C46
    void log(const std::string &msg) override;

  private:
    std::string fName;
    std::ofstream fStream;
};
}; // namespace ns_day4

#endif // NS_DAY4_FILE_LOGGER_H