#ifndef TEXTFILE_ARRAY_H
#define TEXTFILE_ARRAY_H

#include <ILog.hpp>

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <fstream>
#include <iostream>

#include <optional>

namespace ns_day_5 {
class TextfileArray {
  public:
    TextfileArray(std::shared_ptr<ILog> logger, const std::string &filename,
                  const std::string &vectorName);

    int elementsOfMapping();
    std::optional<long> getValueOfIndex(int index);

  private:
    std::shared_ptr<ns_day_5::ILog> logger;
    std::string vectorName;
    int data_index;
    int data_size;
    std::ifstream input_stream;
};
} // namespace ns_day_5

#endif // TEXTFILE_ARRAY_H