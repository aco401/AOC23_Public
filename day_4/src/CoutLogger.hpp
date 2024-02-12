#ifndef NS_DAY4_COUT_LOGGER_H
#define NS_DAY4_COUT_LOGGER_H

#include "ILogger.hpp"
#include <iostream>
#include <string>

namespace ns_day4 {

/**
 * @brief 
 * 
 */
class CoutLogger : public ILogger {
  public:
    void log(const std::string &msg) override;
};
} // namespace ns_day4

#endif // NS_DAY4_COUT_LOGGER_H