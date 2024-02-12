#ifndef NS_DAY_5_FILE_LOGGER_H
#define NS_DAY_5_FILE_LOGGER_H

#include "ILog.hpp"
#include <fstream>
#include <iostream>
#include <string>

namespace d17 { // TODO: check best practices of "using namespace ..."
class FileLogger : public ILog {
  public:
    /**
     * @brief Construct a new Local Logger:: Local Logger object
     *
     * @param str [in] message to log
     *
     * @details Parameter passing: str[in] use f(const X&) to comply with
     * ISOCPP.F15
     */
    explicit FileLogger(
        const std::string &&filename,
        std::ios_base::openmode mode =
            std::ios_base::openmode::_S_app); // explicit -> ISOCPP:C46
    void log(const std::string &msg) override;
    void log(int integer) override;
    void log(unsigned integer) override;
    void log(size_t integer) override;

  private:
    std::string fName;
    std::ofstream fStream;
};
}; // namespace d17

#endif // NS_DAY_5_FILE_LOGGER_H