#ifndef NS_DAY_17_VECTORTOFILE_H
#define NS_DAY_17_VECTORTOFILE_H

#include <VectorOfFile.hpp>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <memory>
#include <string>

#include <ILog.hpp>

namespace d17 {
template <typename IntType> class VectorToFile { // TODO Template not necessarry

  public:
    /**
     * @brief Construct a new Vector To File object. 
     * Enables to write a char array of integers 
     * to an file read by VectorOfFile  
     * 
     * @param logger logger object for debug messages
     * @param readHandler handler to get the file positions
     * @param filename path of the file with the filename
     */
    VectorToFile(std::shared_ptr<d17::ILog> logger,
                 std::shared_ptr<d17::VectorOfFile<IntType>> readHandler,
                 const std::string &filename);
    /**
     * @brief Insert a given value to a given index to the file
     * 
     * @param index position of vector to read from and position of file to write to
     * @param value value to write to file
     * @return bool
     */
    bool insert(size_t index, char value);

    /**
     * @brief Copies all values from readHandler to given file
     * 
     * @return bool whether copying was successfull
     */
    bool copyAllValues();

  private:
    std::shared_ptr<d17::ILog> logger;
    std::ofstream stream;
    std::shared_ptr<d17::VectorOfFile<IntType>> readHandler;
};

// Code is placed in header file to avoid
// undefined reference error due to template usage

template <typename IntType>
VectorToFile<IntType>::VectorToFile(
    std::shared_ptr<d17::ILog> logger,
    std::shared_ptr<d17::VectorOfFile<IntType>> readHandler,
    const std::string &filename)
    : logger{logger}, stream{filename, std::ios::out}, readHandler{
                                                           readHandler} {
    logger->log("VectorToFile object created!");
    assert(!stream.fail());
}

template <typename IntType> // TODO error handling with std::optional
bool VectorToFile<IntType>::VectorToFile::insert(size_t index, char value) {

    assert(index >= 0);
    assert(index < readHandler->size());

    size_t seek_pos = index / readHandler->block_size() + index;
    stream.seekp(seek_pos);

    stream.put(value);

    return true; // TODO
}

template <typename IntType>
bool VectorToFile<IntType>::VectorToFile::copyAllValues() {
    // Insert Values
    for (size_t i = 0; i < readHandler->size(); i++) {
        insert(i, static_cast<char>(readHandler->at(i)) + '0');
    }

    // Insert new lines
    for (size_t i = 1; i <= (readHandler->size() / readHandler->block_size());
         i++) {
        if (i == 1) {
            stream.seekp(i * readHandler->block_size());
        } else {
            stream.seekp(i * (readHandler->block_size()) + i - 1);
        }
        stream.put('\n');
    }
    return false;
}
} // namespace d17

#endif // NS_DAY_17_VECTORTOFILE_H