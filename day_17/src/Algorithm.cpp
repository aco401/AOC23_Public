#include <iostream>
#include <string>

#include <Algorithm.hpp>
#include <Utility.hpp>

#include <IGraph.hpp>
#include <cassert>
#include <iterator>
#include <memory>
#include <queue>
#include <set>
#include <span>
#include <vector>

bool hasSamePastDirections(
    unsigned pastSize, const std::vector<unsigned> &prev,
    const std::vector<d17::IGraph::NeighbourDirection> &prevDir, unsigned u);

bool isInvertedPastDirection(d17::IGraph::NeighbourDirection prevDir,
                             d17::IGraph::NeighbourDirection currDir);

void buildReversePath(std::list<unsigned> &path,
                      const std::vector<unsigned> &prev, unsigned start,
                      unsigned target);

auto NodeVisitedCompare = [](const d17::IGraph::Node &a,
                             const d17::IGraph::Node &b) {
    return a.destinationId == b.destinationId;
};

// TODO Function monolith reduce to smaller functions

// Pseudocode from https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
std::pair<std::list<unsigned>, unsigned>
d17::algorithm::dijkstraGraph(std::shared_ptr<d17::ILog> logger,
                              d17::IGraph &graph, unsigned startId,
                              unsigned targetId) {

    unsigned size = graph.size();

    // {} cant be used for constructing a vector of size "size" !!!!
    std::vector<unsigned> dist(size, UINT_MAX);
    std::vector<unsigned> prev(size, UINT_MAX);

    // Tracks the direction of path
    // To only go 3 time int the same direction
    std::vector<d17::IGraph::NeighbourDirection> prevDir(
        size, d17::IGraph::NeighbourDirection::NONE);

    // Set distance of source/start node to 0
    dist[startId] = 0;
    prev[startId] = 0;

    // Create a priority queue to store the nodes in order of their distance
    // To find Node lowest next distance first
    std::priority_queue<d17::IGraph::Node, std::vector<d17::IGraph::Node>,
                        std::greater<d17::IGraph::Node>>
        priorityQ; //

    // Push source node in the priority queue
    priorityQ.push(IGraph::Node{.destinationId = 0, .cost = dist[startId]});

    while (!priorityQ.empty()) {
        // Get the node with the minimum distance
        logger->log("PriorityQ Top: " + priorityQ.top().to_string());
        unsigned u = priorityQ.top().destinationId;
        priorityQ.pop();

        //assert(u < size);

        // Shortest Path for target Node found
        // if (u == targetId) {
        //     break;
        // }

        // Update the distance of its neighbors
        // TODO Is iterator copied and should i use auto &
        for (auto it = graph.getNeighbours(u, d17::IGraph::IteratorPos::Start);
             it != graph.getNeighbours(u, d17::IGraph::IteratorPos::End);
             it++) {

            unsigned v = it->destinationId;
            unsigned alt = it->cost + dist[u];

            // If the distance of the neighbor is more than the distance of the source node + the edge weight
            if (dist[v] > alt) {

                // Special: Going into the same direction 3 times is not allowed
                if (!(hasSamePastDirections(2, prev, prevDir, u) &&
                      it->dir == prevDir[u]) &&
                    !isInvertedPastDirection(prevDir[u], it->dir)) {
                    // Update the distance of the neighbor
                    dist[v] = alt;
                    prev[v] = u;

                    // To track the direction of the path
                    prevDir[v] = it->dir;

                    // Push the neighbor in the priority queue
                    priorityQ.push(
                        IGraph::Node{.destinationId = v, .cost = dist[v]});
                }
            }
        }
    }

    // Build path from source to target for returning
    std::list<unsigned> path{};
    buildReversePath(path, prev, startId, targetId);

    logger->log(d17::vectorToString(dist));
    logger->log(d17::vectorToString(prev));
    logger->log(d17::vectorToString(prevDir));

    return std::make_pair(path, dist[targetId]);
};

void buildReversePath(std::list<unsigned> &path,
                      const std::vector<unsigned> &prev, unsigned start,
                      unsigned target) {

    assert(path.size() == 0);

    unsigned u = target;
    if (prev[u] != UINT_MAX) {
        while (u != start) {
            path.push_front(u);
            u = prev[u];
        }
        // Finally push startID
        path.push_front(u);
    }
}

/**
 * @brief Has path (prev) from node with id u the same direction for the last pastSize Nodes
 * 
 * @param pastSize Amount of Nodes in path to consider 
 * @param prev path with node ids
 * @param prevDir direction of node ids
 * @param u nodeId to view the path from
 * @return true last pastSize of nodes in path are in the same direction
 * @return false last pastSize of nodes in path are not in the same direction
 */
bool hasSamePastDirections(
    unsigned pastSize, const std::vector<unsigned> &prev,
    const std::vector<d17::IGraph::NeighbourDirection> &prevDir, unsigned u) {

    std::list<d17::IGraph::NeighbourDirection> pathDir{};
    unsigned id = u;
    for (size_t i = 0; i < pastSize; i++) {

        if (pathDir.size() > 0 && prevDir[u] != pathDir.front()) {
            pathDir.clear();
        }
        pathDir.push_front(prevDir[u]);
        u = prev[u];
    }

    return pathDir.size() >= pastSize;
}

/**
 * @brief Checks whether the path is going back, which is not allowed
 * 
 * @param prevDir direction from last to current node in path
 * @param currDir direction from current to next node in path
 * @return true last pastSize of nodes in path are in the same direction
 * @return false last pastSize of nodes in path are not in the same direction
 */
bool isInvertedPastDirection(d17::IGraph::NeighbourDirection prevDir,
                             d17::IGraph::NeighbourDirection currDir) {

    bool ret = false;
    switch (currDir) {
    case d17::IGraph::NeighbourDirection::UP:
        ret = (prevDir == d17::IGraph::NeighbourDirection::DOWN);
        break;
    case d17::IGraph::NeighbourDirection::DOWN:
        ret = (prevDir == d17::IGraph::NeighbourDirection::UP);
        break;
    case d17::IGraph::NeighbourDirection::LEFT:
        ret = (prevDir == d17::IGraph::NeighbourDirection::RIGHT);
        break;
    case d17::IGraph::NeighbourDirection::RIGHT:
        ret = (prevDir == d17::IGraph::NeighbourDirection::LEFT);
        break;
    }

    return ret;
}
