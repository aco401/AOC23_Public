#include <Data_Example.hpp>

namespace ns_day_5 {

namespace data_example { // Might actually be better with qualified name for each array

const std::string file{"data_example.txt"};

std::array<int, 6> seedToSoil{50, 98, 2, 52, 50, 48};

std::array<int, 9> soilToFertilizer{0, 15, 37, 37, 52, 2, 39, 0, 15};

std::array<int, 12> fertilizerToWater{49, 53, 8, 0, 11, 42, 42, 0, 7, 57, 7, 4};

std::array<int, 6> waterToLight{88, 18, 7, 18, 25, 70};

std::array<int, 9> lightToTemperature{45, 77, 23, 81, 45, 19, 68, 64, 13};

std::array<int, 6> temperatureToHumidity{0, 69, 1, 1, 0, 69};

std::array<int, 6> HumidityToLocation{60, 56, 37, 56, 93, 4};

} // namespace data_example

} // namespace ns_day_5