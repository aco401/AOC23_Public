#ifndef NS_DAY_5_DUMMYLOGGER_H
#define NS_DAY_5_DUMMYLOGGER_H

#include "ILog.hpp"
#include <iostream>
#include <string>

namespace ns_day_5 {
class DummyLogger : public ILog {
  public:
    void log(const std::string &msg) override{};
};
} // namespace ns_day_5

#endif // NS_DAY_5_DUMMYLOGGER_H