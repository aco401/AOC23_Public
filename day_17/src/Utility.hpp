#ifndef UTILITY_PARSER_H
#define UTILITY_PARSER_H

#include <IGraph.hpp>
#include <list>
#include <map>
#include <queue>
#include <string>
#include <vector>

namespace d17 {
/**
 * @brief Convert space separated string of numbers to a vector
 * 
 * @param in [in] [prec: !in.empty()] will be parsed and consumed 
 * @param out [in/out] [prec: out.empty()] container to save parsed values to
 */
void getVectorFromString(const std::string &in, std::vector<unsigned> &out);

std::string mapToString(const std::map<unsigned, unsigned> &m);
std::string vectorToString(const std::vector<unsigned> &v);
std::string
vectorToString(const std::vector<d17::IGraph::NeighbourDirection> &v);
std::string listToString(const std::list<unsigned> &v);
std::string
queueTopToString(const std::priority_queue<d17::IGraph::Node> &queue);
} // namespace d17

#endif // UTILITY_PARSER_H