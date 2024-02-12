#include <GraphAdjacentList.hpp>
#include <GraphFileHandler.hpp>
#include <VectorOfFile.hpp>
#include <VectorToFile.hpp>

d17::GraphFileHandler::GraphFileHandler(std::shared_ptr<d17::ILog> logger)
    : logger{logger} {}

std::unique_ptr<d17::IGraph>
d17::GraphFileHandler::createGraph(const std::string &filename) {
    d17::VectorOfFile<unsigned> vecFile{logger, filename};

    std::unique_ptr<d17::IGraph> graph =
        std::make_unique<d17::GraphAdjacentList>(logger, vecFile.size());

    for (size_t i = 0; i < vecFile.size(); i++) {

        // handle right verteces
        if ((i % (vecFile.block_size() - 1)) != 0) {
            graph->addEdge(i, i + 1, vecFile.at(i + 1),
                           d17::IGraph::NeighbourDirection::RIGHT);
        }

        // handle left verteces
        if ((i % vecFile.block_size()) != 0) {
            graph->addEdge(i, i - 1, vecFile.at(i - 1),
                           d17::IGraph::NeighbourDirection::LEFT);
        }

        // handle up verteces
        if (static_cast<int>(i) - static_cast<int>(vecFile.block_size()) >= 0) {
            graph->addEdge(i, i - vecFile.block_size(),
                           vecFile.at(i - vecFile.block_size()),
                           d17::IGraph::NeighbourDirection::UP);
        }

        // handle down verteces
        if ((vecFile.size() - i > vecFile.block_size())) {
            graph->addEdge(i, i + vecFile.block_size(),
                           vecFile.at(i + vecFile.block_size()),
                           d17::IGraph::NeighbourDirection::DOWN);
        }
    }

    return std::move(graph);
}
void d17::GraphFileHandler::createFile(const std::list<unsigned> &path,
                                       const std::string &inputFilename,
                                       const std::string &outputFilename) {
    std::shared_ptr<d17::VectorOfFile<unsigned>> vecFromFile =
        std::make_shared<d17::VectorOfFile<unsigned>>(logger, inputFilename);
    d17::VectorToFile<unsigned> vecOutFile{logger, vecFromFile, outputFilename};

    vecOutFile.copyAllValues();

    for (auto index : path) {
        vecOutFile.insert(index, '~');
    }
}