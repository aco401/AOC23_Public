#ifndef NS_DAY_5_DATA_EXAMPLE_H
#define NS_DAY_5_DATA_EXAMPLE_H

#include <array>
#include <string>

namespace ns_day_5 {

namespace data_example {

extern const std::string file;

//extern std::array<int, 4> seed;

extern std::array<int, 6> seedToSoil;

extern std::array<int, 9> soilToFertilizer;

extern std::array<int, 12> fertilizerToWater;

extern std::array<int, 6> waterToLight;

extern std::array<int, 9> lightToTemperature;

extern std::array<int, 6> temperatureToHumidity;

extern std::array<int, 6> HumidityToLocation;
} // namespace data_example

} // namespace ns_day_5

#endif // NS_DAY_5_DATA_EXAMPLE_H