#include <GraphAdjacentList.hpp>
#include <cassert>

d17::GraphAdjacentList::GraphAdjacentList(std::shared_ptr<d17::ILog> logger,
                                          unsigned nodes)
    : logger{logger}, nodes{nodes}, adjList{} {
    for (size_t i = 0; i < nodes; i++) {
        NodeList temp;
        //temp.sourceId = i;
        adjList.push_back(temp);
    }
};

void d17::GraphAdjacentList::addEdge(unsigned source, unsigned dest,
                                     unsigned cost, NeighbourDirection dir) {
    assert(source >= 0);
    assert(source < nodes);

    assert(dest >= 0);
    assert(dest < nodes);

    assert(cost >= 0);
    logger->log("GraphAdjacentList::addEdge: {source, dest, cost}: {" +
                std::to_string(source) + ", " + std::to_string(dest) + ", " +
                std::to_string(cost) +
                std::to_string(static_cast<unsigned>(dir)) + "}");
    //adjList[source].sourceId = source;
    adjList[source].neighbours.push_back(
        d17::IGraph::Node{.destinationId = dest, .cost = cost, .dir = dir});
};

unsigned d17::GraphAdjacentList::size() const { return nodes; }

std::vector<d17::IGraph::Node>::iterator d17::GraphAdjacentList::getNeighbours(
    unsigned source, d17::IGraph::IteratorPos pos) { // TODO use iterator
    logger->log("GraphAdjacentList::getNeighbours: {source}: " +
                std::to_string(source));
    assert(source >= 0);
    assert(source < nodes);
    switch (pos) {
    default:
    case d17::IGraph::IteratorPos::Start:
        return adjList[source].neighbours.begin();
        break;
    case d17::IGraph::IteratorPos::End:
        return adjList[source].neighbours.end();
        break;
    }
};

void d17::GraphAdjacentList::displayEdges(){};