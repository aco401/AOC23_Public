#ifndef AOC_D19_TRACEEXCEPTION
#define AOC_D19_TRACEEXCEPTION

#include <string>

#define throw_message(msg)                                                     \
    (std::string(msg) + " - in File: " + __FILE__ + ":" +                      \
     std::to_string(__LINE__))

#endif