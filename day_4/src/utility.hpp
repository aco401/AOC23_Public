#ifndef UTILITY_PARSER_H
#define UTILITY_PARSER_H

#include <string>
#include <vector>

namespace ns_day4 {
/**
 * @brief Convert space separated string of numbers to a vector
 * 
 * @param in [in] [prec: !in.empty()] will be parsed and consumed 
 * @param out [in/out] [prec: out.empty()] container to save parsed values to
 */
void getVectorFromString(const std::string &in, std::vector<int> &out);
} // namespace ns_day4

#endif // UTILITY_PARSER_H