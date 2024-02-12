#ifndef NS_DAY_17_GRAPHFILEHANDLER_H
#define NS_DAY_17_GRAPHFILEHANDLER_H

#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <string>

#include <IGraph.hpp>
#include <ILog.hpp>
#include <VectorOfFile.hpp>

namespace d17 {
class GraphFileHandler {

  public:
    GraphFileHandler(std::shared_ptr<d17::ILog> logger);
    std::unique_ptr<IGraph> createGraph(const std::string &filename);
    void createFile(const std::list<unsigned> &path,
                    const std::string &inputFilename,
                    const std::string &outputFilename);

  private:
    std::shared_ptr<d17::ILog> logger;
};
} // namespace d17

#endif // NS_DAY_17_GRAPHFILEHANDLER_H