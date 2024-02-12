#ifndef NS_DAY_17_GRAPHADJACENTLIST_H
#define NS_DAY_17_GRAPHADJACENTLIST_H

#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <string>
#include <vector>

#include <IGraph.hpp>
#include <ILog.hpp>

namespace d17 {

class GraphAdjacentList : public IGraph {

    struct NodeList {
        std::vector<IGraph::Node> neighbours;
    };

  public:
    explicit GraphAdjacentList(std::shared_ptr<d17::ILog> logger,
                               unsigned nodes);
    void addEdge(unsigned source, unsigned dest, unsigned cost,
                 NeighbourDirection dir) override;
    std::vector<IGraph::Node>::iterator
    getNeighbours(unsigned source, IGraph::IteratorPos pos) override;
    unsigned size() const override;
    void displayEdges() override;

  private:
    std::shared_ptr<d17::ILog> logger;
    unsigned nodes;
    std::vector<NodeList> adjList;
};
} // namespace d17

#endif // NS_DAY_17_GRAPHADJACENTLIST_H