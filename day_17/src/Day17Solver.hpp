#ifndef NS_DAY_17_SOLVER_H
#define NS_DAY_17_SOLVER_H

#include "ILog.hpp"

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace d17 { // Best practice: If your class type has no data members, prefer using a namespace

int solve_file(std::shared_ptr<d17::ILog> logger, const std::string &,
               unsigned startId, unsigned targetId);

} // namespace d17

#endif // NS_DAY_17_SOLVER_H