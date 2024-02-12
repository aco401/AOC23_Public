#ifndef AOC_D19_FILELINEHANDLER_H
#define AOC_D19_FILELINEHANDLER_H

#include "../logging/ILog.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <memory>
#include <optional>
#include <string>

namespace d19 {

class FileLineHandler {

  public:
    class iterator {

        std::ifstream iter_stream;
        size_t iter_streamCurrPos;
        size_t iter_fileHash;
        char iter_delimiter;
        size_t iter_blockStartPos;
        size_t iter_blockEndPos;

      public:
        enum class LineDirection : uint8_t { next, prior };
        enum class BlockPos : uint8_t { start, end };
        /**
         * @brief Construct a new iterator object
         * 
         * @param filename 
         * @param pos 
         * @param delimiter 
         * @param sizeBlock 
         * @param blockStartPos position of first char in block of first line
         * @param blockEndPos position of last char in block of last line
         */
        explicit iterator(const std::string &filename, BlockPos pos,
                          char delimiter, size_t blockStartPos,
                          size_t blockEndPos);
        iterator &operator++();
        iterator &operator--();
        [[nodiscard]] bool operator==(const iterator &other) const;
        [[nodiscard]] bool operator!=(const iterator &other) const;
        [[nodiscard]] std::string operator*();

      private:
        void MoveToDelimiter(LineDirection dir);
        void MoveLine(LineDirection dir);
        void MoveToPos(BlockPos dir);
    };

    /**
     * @brief Construct a new File Handler object to traverse file lines.
     * 
     * @param logger logger object for debug messages
     * @param filename path of the file with the filename
     * @param delimiter char to separate lines with
     * @param index index of data block to iterate over, 
     * blocks are separated by two following delimiters
     * 
     * @exception std::invalid_argument if filename is empty
     * @exception std::invalid_argument if index is bigger than blocks present in file
     */
    FileLineHandler(std::shared_ptr<d19::ILog> logger,
                    const std::string &filename, char delimiter, size_t index);

    /**
     * @brief length of file in lines
     * 
     * @return size_t the size
     */
    size_t size();

    [[nodiscard]] iterator begin();

    /**
     * @brief Returns an iterator referring to the past-the-end element in the vector container.
     * 
     * @return iterator 
     */
    [[nodiscard]] iterator end();

  private:
    /**
     * @brief 
     * 
     * @return std::tuple<size_t, size_t, size_t> 
     * 
     * @exception std::invalid_argument if index is bigger than blocks present in file
     */
    [[nodiscard]] std::tuple<size_t, size_t, size_t> findBlock();
    void findBlockStart(std::ifstream &stream,
                        std::tuple<size_t, size_t, size_t> &data,
                        size_t &lastPos);
    void findBlockEnd(std::ifstream &stream,
                      std::tuple<size_t, size_t, size_t> &data,
                      size_t &lastPos);

  private:
    std::shared_ptr<d19::ILog> logger;
    std::string filename;
    char delimiter;
    size_t blockSize;
    size_t index;

    //position of first char in block of first line
    size_t blockStartPos;

    //position of last char in block of last line
    size_t blockEndPos;
};

} // namespace d19

#endif // AOC_D19_FILELINEHANDLER_H