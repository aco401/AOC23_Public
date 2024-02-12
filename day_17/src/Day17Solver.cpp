#include <Day17Solver.hpp>
#include <DummyLogger.hpp>
#include <FileLogger.hpp>

#include <fstream>

#include <array>
#include <limits>

#include <utility.hpp>

#include <GraphFileHandler.hpp>

#include <Algorithm.hpp>

int d17::solve_file(std::shared_ptr<d17::ILog> logger,
                    const std::string &filename, unsigned startId,
                    unsigned targetId) {

    logger->log("Solving File: " + filename);

    std::unique_ptr<d17::GraphFileHandler> graphFactory =
        std::make_unique<d17::GraphFileHandler>(logger);

    std::unique_ptr<d17::IGraph> graph = graphFactory->createGraph(filename);

    std::pair<std::list<unsigned>, unsigned> ret =
        d17::algorithm::dijkstraGraph(logger, *graph.get(), startId, targetId);

    logger->log("Path: " + d17::listToString(ret.first));
    logger->log("Cost from Node " + std::to_string(startId) + " to " +
                std::to_string(targetId) + ": " + std::to_string(ret.second));

    // Write path to file
    std::string outputFileName =
        filename.substr(0, filename.size() - 4) + "_path.txt";

    graphFactory->createFile(ret.first, filename, outputFileName);

    return ret.second;
};