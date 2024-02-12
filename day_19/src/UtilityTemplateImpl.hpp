#ifndef AOC_D19_UTILITY_TEMPLATE_IMPL_H
#define AOC_D19_UTILITY_TEMPLATE_IMPL_H

#include <functional>
#include <list>
#include <map>
#include <queue>
#include <ranges>
#include <string>
#include <vector>

namespace d19 {

// ------------------- String Conversions ---------------------

template <typename T>
[[nodiscard]] std::string vectorToString(const std::vector<T> &vec,
                                         std::function<std::string(T)> func) {
    std::string ret{""};
    if (!vec.empty()) {
        ret += "{";
        for (auto it = vec.begin(); it != vec.end(); ++it) {
            ret += func(*it) + ((it == (vec.end() - 1)) ? "" : ",");
        }
        ret += "}";
    }
    return ret;
}

// ---------------------- Container Utility --------------------------

template <typename ValueType>
unsigned findUnusedKey(const std::map<unsigned, ValueType> &map) {
    if (map.empty()) {
        return 0;
    }
    auto keyView = std::views::keys(map);
    std::vector<unsigned> keys{keyView.begin(), keyView.end()};
    return findUnusedKey(keys);
};

} // namespace d19

#endif // AOC_D19_UTILITY_TEMPLATE_IMPL_H