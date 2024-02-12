#ifndef NS_DAY_17_VECTOROFFILE_H
#define NS_DAY_17_VECTOROFFILE_H

#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <memory>
#include <string>

#include <DummyLogger.hpp>
#include <ILog.hpp>

const size_t undefined_size = UINT_MAX;

namespace d17 {
template <typename IntType> class VectorOfFile {

  public:
    /**
     * @brief Construct a new Vector Of File object. 
     * Enables to read a file char array of integers 
     * as an read-only integer array / vector. 
     * 
     * @param logger logger object for debug messages
     * @param filename path of the file with the filename
     * @param lineLength length of a single line in the file
     */
    VectorOfFile(std::shared_ptr<d17::ILog> logger, const std::string &filename,
                 size_t lineLength);

    /**
     * @brief Construct a new Vector Of File object.
     * Enables to read a file char array of integers 
     * as an read-only integer array / vector. 
     * 
     * @param logger logger object for debug messages
     * @param filename path of the file with the filename
     */
    VectorOfFile(std::shared_ptr<d17::ILog> logger,
                 const std::string &filename);

    /**
     * @brief Construct a new Vector Of File object.
     * Enables to read a file char array of integers 
     * as an read-only integer array / vector. 
     * 
     * @param filename path of the file with the filename
     */
    explicit VectorOfFile(const std::string &filename);

    /**
     * @brief Returns the integer at the given index
     * 
     * @param index position of vector to return integer for
     * @return * IntType 
     */
    IntType at(size_t index);

    /**
     * @brief Size of integers in file vector
     * 
     * @return size_t the size
     */
    size_t size();

    /**
     * @brief Size of a single row of integers
     * 
     * @return size_t the size
     */
    size_t block_size();

  private:
    std::shared_ptr<d17::ILog> logger;
    std::ifstream stream;
    size_t lineLength;
};

// Is here to not at undefined reference error

template <typename IntType>
VectorOfFile<IntType>::VectorOfFile(std::shared_ptr<d17::ILog> logger,
                                    const std::string &filename,
                                    size_t lineLength)
    : logger{logger}, stream{filename, std::ios::in}, lineLength{lineLength} {
    logger->log("VectorOfFile object created!");
    assert(!stream.fail());
}

template <typename IntType>
VectorOfFile<IntType>::VectorOfFile(std::shared_ptr<d17::ILog> logger,
                                    const std::string &filename)
    : logger{logger}, stream{filename, std::ios::in}, lineLength{
                                                          undefined_size} {
    logger->log("VectorOfFile object created!");
    assert(!stream.fail());
    lineLength = block_size();
}

template <typename IntType>
VectorOfFile<IntType>::VectorOfFile(const std::string &filename)
    : logger{std::make_shared<d17::DummyLogger>()},
      stream{filename, std::ios::in}, lineLength{undefined_size} {

    assert(!stream.fail());
    lineLength = block_size();
}

template <typename IntType> // TODO error handling with std::optional
IntType VectorOfFile<IntType>::VectorOfFile::at(size_t index) {
    assert(index >= 0);
    assert(index < size()); // TODO might be optimized via data member

    size_t seek_pos = index / lineLength + index;
    stream.seekg(seek_pos);

    IntType ret = static_cast<IntType>(stream.peek() - '0');
    assert(ret >= 0);
    assert(ret <= 9);
    return ret;
}

template <typename IntType> size_t VectorOfFile<IntType>::VectorOfFile::size() {
    stream.seekg(0, std::ios_base::end);
    size_t seek_pos = stream.tellg();
    logger->log(seek_pos);
    return seek_pos - (seek_pos / (lineLength + 1));
}

template <typename IntType>
size_t VectorOfFile<IntType>::VectorOfFile::block_size() {
    unsigned block_size = 0;

    if (lineLength == undefined_size) {
        // Look for line feed character
        while (stream.peek() != (char)10) {
            block_size += 1;
            stream.seekg(1, std::ios_base::cur);
        }
    } else {
        block_size = lineLength;
    }

    return block_size;
}
} // namespace d17

#endif // NS_DAY_17_VECTOROFFILE_H