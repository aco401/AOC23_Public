#ifndef NS_DAY_17_IGRAPH_H
#define NS_DAY_17_IGRAPH_H

#include <iostream>
#include <string>

#include <iterator>
#include <list>
#include <vector>

namespace d17 { // TODO ns_ style convention
class IGraph {
  public:
    enum class NeighbourDirection { UP, DOWN, LEFT, RIGHT, NONE };

    struct Node {
        unsigned destinationId;
        unsigned cost;
        NeighbourDirection dir;

        friend bool operator==(const Node &l, const Node &r) {
            return (l.destinationId == r.destinationId) && (l.cost == r.cost) &&
                   (l.dir == l.dir);
        };

        friend bool operator<(const Node &l, const Node &r) {
            return l.cost < r.cost; // keep the same order
        };

        friend bool operator>(const Node &l, const Node &r) {
            return l.cost > r.cost; // keep the same order
        };

        std::string to_string() const {
            return "{Node: .id: " + std::to_string(destinationId) +
                   ", .cost: " + std::to_string(cost) +
                   ", .dir: " + std::to_string(static_cast<int>(dir)) + "}";
        }
    };

    enum class IteratorPos {
        Start,
        End,
    };

    //  == INSTANTIATION ==
  protected:
    //  -- Constructors --
    IGraph() = default;
    IGraph(const IGraph &) = default;
    IGraph(IGraph &&) = default;

  public:
    //  -- Destructors --
    virtual ~IGraph(){};

    //  == OPERATORS ==
  protected:
    //  -- Assignment --
    IGraph &operator=(const IGraph &) = default;
    IGraph &operator=(IGraph &&) = default;

    //  == METHODS ==
  public:
    virtual void addEdge(unsigned source, unsigned dest, unsigned cost,
                         NeighbourDirection dir) = 0;
    virtual std::vector<Node>::iterator getNeighbours(unsigned source,
                                                      IteratorPos pos) = 0;
    // TODO return pair of [begin, end]
    // TODO template parameter vector<Node>
    virtual unsigned size() const = 0;
    virtual void displayEdges() = 0;
};
} // namespace d17

#endif // NS_DAY_17_IGRAPH_H