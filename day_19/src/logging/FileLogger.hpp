#ifndef AOC_D19_FILE_LOGGER_H
#define AOC_D19_FILE_LOGGER_H

#include "ILog.hpp"
#include <fstream>
#include <iostream>
#include <string>

namespace d19 {
class FileLogger : public ILog {
  public:
    /**
     * @brief Construct a new Local Logger:: Local Logger object
     *
     * @param str [in] message to log
     *
     * @details Parameter passing: str[in] use f(const X&) to comply with
     * ISOCPP.F15
     * 
     * @exception std::invalid_argument if @a str is empty
     * @exception std::ios_base::failure if opened stream to file is !good()
     * 
     */
    explicit FileLogger(
        const std::string &filename,
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
}; // namespace d19

#endif // AOC_D19_FILE_LOGGER_H