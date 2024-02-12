#ifndef NS_DAY_5_DUMMYLOGGER_H
#define NS_DAY_5_DUMMYLOGGER_H

#include "ILog.hpp"
#include <iostream>
#include <string>

namespace d17 {
class DummyLogger : public ILog {
  public:
    void log(const std::string &msg) override{};
    void log(int integer) override{};
    void log(unsigned integer) override{};
    void log(size_t integer) override{};
};
} // namespace d17

#endif // NS_DAY_5_DUMMYLOGGER_H