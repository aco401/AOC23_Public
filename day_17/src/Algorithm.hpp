#ifndef NS_DAY_17_ALGORITHM_H
#define NS_DAY_17_ALGORITHM_H

#include <iostream>
#include <string>

#include <IGraph.hpp>
#include <ILog.hpp>
#include <iterator>
#include <list>
#include <memory>
#include <span>

namespace d17 {
namespace algorithm {

/**
 * @brief Algorithm to find the shortest path in a graph from Node startId to targetId.
 * Only goes 3 times in the same direction.
 * 
 * @param logger to log debugging data
 * @param graph graph object to find the path in
 * @param startId 
 * @param targetId 
 * @return std::pair<std::list<unsigned>, unsigned> 
 */
std::pair<std::list<unsigned>, unsigned>
dijkstraGraph(std::shared_ptr<d17::ILog> logger, d17::IGraph &graph,
              unsigned startId, unsigned targetId);

} // namespace algorithm
} // namespace d17

#endif // NS_DAY_17_ALGORITHM_H