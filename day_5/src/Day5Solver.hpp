#ifndef NS_DAY_5_SOLVER_H
#define NS_DAY_5_SOLVER_H

#include "ILog.hpp"
#include <CategoryMapInputFactory.hpp>
#include <ICategoryMap.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace ns_day_5 { // Best practice: If your class type has no data members, prefer using a namespace
std::pair<int, int> solve_example(std::shared_ptr<ns_day_5::ILog> logger);
std::pair<int, int> solve_file(std::shared_ptr<ns_day_5::ILog> logger,
                               const std::string &);
std::pair<int, int> findLowestLocation(std::shared_ptr<ns_day_5::ILog> logger,
                                       const std::map<int, int> &m);
int findLocationForSeed(
    std::shared_ptr<ns_day_5::ILog> logger, const int seed,
    const std::map<ns_day_5::CategoryMapEnum::Type,
                   std::unique_ptr<ns_day_5::ICategoryMap>> &categories);

} // namespace ns_day_5

#endif // NS_DAY_5_SOLVER_H