#ifndef AOC_D19_COUTLOGGER_H
#define AOC_D19_COUTLOGGER_H

#include "ILog.hpp"
#include <iostream>
#include <string>

namespace d19 {
class CoutLogger : public ILog {

  public:
    void log(const std::string &msg) override {
        std::cout << msg << std::endl;
    };
    void log(int integer) override {
        std::cout << std::to_string(integer) << std::endl;
    };
    void log(unsigned integer) override {
        std::cout << std::to_string(integer) << std::endl;
    };
    void log(size_t integer) override {
        std::cout << std::to_string(integer) << std::endl;
    };
};
} // namespace d19

#endif // AOC_D19_COUTLOGGER_H