#ifndef AOC_D19_DUMMYLOGGER_H
#define AOC_D19_DUMMYLOGGER_H

#include "ILog.hpp"
#include <iostream>
#include <string>

namespace d19 {
class DummyLogger : public ILog {
  public:
    void log(const std::string &msg) override{};
    void log(int integer) override{};
    void log(unsigned integer) override{};
    void log(size_t integer) override{};
};
} // namespace d19

#endif // AOC_D19_DUMMYLOGGER_H