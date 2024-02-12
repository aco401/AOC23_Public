#include <iostream>

#include <Day17Solver.hpp>

#include <DummyLogger.hpp>
#include <FileLogger.hpp>

#include <Algorithm.hpp>
#include <GraphAdjacentList.hpp>
#include <GraphFileHandler.hpp>
#include <Utility.hpp>
#include <cassert>

#include <VectorOfFile.hpp>

constexpr const char *file_data_example = "../src/data_example.txt";
constexpr const char *file_data_example_path = "../src/data_example_path.txt";

constexpr const char *file_data_input = "../src/data_input.txt";

constexpr const char *log_file = "log.txt";

int main(int, char **) {
    // std::shared_ptr<d17::ILog> logger =
    //     std::make_shared<d17::FileLogger>(log_file);
    std::shared_ptr<d17::ILog> logger = std::make_shared<d17::DummyLogger>();
    {

        d17::VectorOfFile<unsigned> filehandle(logger, file_data_example);
        int solution = d17::solve_file(logger, file_data_example, 0,
                                       filehandle.size() - 1);
        std::cout << "File " + std::string(file_data_example) +
                         ": The path found has a cost of " +
                         std::to_string(solution)
                  << "\n";
    }
    //return 0;
    {
        d17::VectorOfFile<unsigned> filehandle =
            d17::VectorOfFile<unsigned>(logger, file_data_input);
        int solution =
            d17::solve_file(logger, file_data_input, 0, filehandle.size() - 1);
        std::cout << "File " + std::string(file_data_input) +
                         ": The path found has a cost of " +
                         std::to_string(solution)
                  << "\n";
    }

    // solution = d17::solve_file(logger, file_data_input);
    // // std::cout << "File " + std::string(file_data_input) + ": The seed " +
    // //                  std::to_string(solution.first) +
    // //                  " has the lowest location of " +
    // //                  std::to_string(solution.second)
    // //           << "\n";

    // int size = 9;

    // d17::GraphAdjacentList graph(logger, size);
    // graph.addEdge(0, 1, 4, d17::IGraph::NeighbourDirection::UP);
    // graph.addEdge(0, 2, 23, d17::IGraph::NeighbourDirection::DOWN);
    // graph.addEdge(0, 3, 100, d17::IGraph::NeighbourDirection::RIGHT);
    // graph.addEdge(1, 4, 45, d17::IGraph::NeighbourDirection::RIGHT);
    // graph.addEdge(2, 5, 1, d17::IGraph::NeighbourDirection::RIGHT);
    // graph.addEdge(5, 3, 37, d17::IGraph::NeighbourDirection::UP);
    // graph.addEdge(4, 3, 20, d17::IGraph::NeighbourDirection::DOWN);

    // graph.addEdge(3, 5, 38, d17::IGraph::NeighbourDirection::DOWN);
    // graph.addEdge(3, 8, 100, d17::IGraph::NeighbourDirection::RIGHT);
    // graph.addEdge(5, 6, 1, d17::IGraph::NeighbourDirection::RIGHT);
    // graph.addEdge(6, 7, 2, d17::IGraph::NeighbourDirection::RIGHT);
    // graph.addEdge(7, 8, 2, d17::IGraph::NeighbourDirection::UP);

    // std::pair<std::list<int>, int> ret =
    //     d17::algorithm::dijkstraGraph(logger, graph, 0, 3);
    // logger->log(d17::listToString(ret.first));

    // ret = d17::algorithm::dijkstraGraph(logger, graph, 0, 8);
    // logger->log(d17::listToString(ret.first));

    // d17::VectorOfFile<int> fileVec{logger, "../src/data_example.txt", 13};
    // logger->log(std::to_string(fileVec.at(13)));

    return 0;
}
