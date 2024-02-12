#ifndef NS_DAY4_DUMMYLOGGER_H
#define NS_DAY4_DUMMYLOGGER_H

#include "ILogger.hpp"
#include <iostream>
#include <string>

namespace ns_day4 {
class DummyLogger : public ILogger {
  public:
    void log(const std::string &msg) override{};
};
} // namespace ns_day4

#endif // NS_DAY4_DUMMYLOGGER_H