#ifndef AOC_D19_UTILITY_H
#define AOC_D19_UTILITY_H

#include <functional>
#include <list>
#include <map>
#include <queue>
#include <ranges>
#include <string>
#include <vector>

namespace d19 {

// ------------------- String Conversions ---------------------

/**
 * @brief Convert space separated string of numbers to a vector
 * 
 * @param in [in] [prec: !in.empty()] will be parsed and consumed 
 * @param out [in/out] [prec: out.empty()] container to save parsed values to
 */
void getVectorFromString(const std::string &input, std::vector<unsigned> &out);

[[nodiscard]] std::string mapToString(const std::map<unsigned, unsigned> &map);
[[nodiscard]] std::string vectorToString(const std::vector<unsigned> &vec);
template <typename T>
[[nodiscard]] std::string vectorToString(const std::vector<T> &vec,
                                         std::function<std::string(T)> func);
[[nodiscard]] std::string listToString(const std::list<unsigned> &list);

// ---------------------- Container Utility --------------------------

/**
     * @brief Find the first unused value in @a keys
     * 
     * @param keys 
     * @return unsigned 
     * 
     * @warning will sort @a keys
     * @warning untested
     */
[[nodiscard]] unsigned findUnusedKey(std::vector<unsigned> &keys);

/**
     * @brief Find the first unused key in @a map
     * 
     * @param map 
     * @return unsigned
     * 
     * @warning untested
     */
template <typename ValueType>
[[nodiscard]] unsigned findUnusedKey(const std::map<unsigned, ValueType> &map);

} // namespace d19

#include <UtilityTemplateImpl.hpp>

#endif // AOC_D19_UTILITY_H