#include <FileLineHandler.hpp>

#include <ILog.hpp>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <memory>
#include <optional>
#include <string>

#include <../TraceException.hpp>
#include <DummyLogger.hpp>

d19::FileLineHandler::FileLineHandler(std::shared_ptr<d19::ILog> logger,
                                      const std::string &filename,
                                      char delimiter, size_t index)
    : logger{logger}, filename{filename}, delimiter{delimiter},
      blockSize{SIZE_MAX}, index{index}, blockStartPos{SIZE_MAX},
      blockEndPos{SIZE_MAX} {

    // Preserve class invariance
    if (filename.empty()) {
        throw std::invalid_argument(throw_message("filename is empty()!"));
    }

    std::tie(blockStartPos, blockEndPos, blockSize) = findBlock();

    if (logger) {
        logger->log("FileLineHandler object created!");
        logger->log("blockStartPos: " + std::to_string(blockStartPos));
        logger->log("blockEndPos: " + std::to_string(blockEndPos));
        logger->log("blockSize: " + std::to_string(blockSize));
    }
}

size_t d19::FileLineHandler::FileLineHandler::size() {
    // Has to be calculated prior to this call by findBlock()
    assert(blockSize != SIZE_MAX);

    return blockSize;
}

d19::FileLineHandler::iterator d19::FileLineHandler::begin() {
    return d19::FileLineHandler::iterator{
        filename, d19::FileLineHandler::iterator::BlockPos::start, delimiter,
        blockStartPos, blockEndPos};
};

d19::FileLineHandler::iterator d19::FileLineHandler::end() {
    return d19::FileLineHandler::iterator{
        filename, d19::FileLineHandler::iterator::BlockPos::end, delimiter,
        blockStartPos, blockEndPos};
};

// TODO Code Review - could be smaller, more modularized
void d19::FileLineHandler::findBlockStart(
    std::ifstream &stream, std::tuple<size_t, size_t, size_t> &data,
    size_t &lastPos) {

    std::string lastLine{""};
    std::string currLine{""};

    stream.seekg(0, std::ios_base::beg);

    size_t visitedBlock{SIZE_MAX};

    // Find start of block
    while (std::getline(stream, currLine, delimiter).good()) {

        if (logger) {
            logger->log("start block: currLine: " + currLine);
        }

        if (lastLine.size() == 0 && currLine.size() != 0) {
            if (visitedBlock != SIZE_MAX) {
                visitedBlock += 1;
            } else {
                visitedBlock = 0;
            }
            // Assign blockStartPos
            std::get<0>(data) = lastPos;
        }

        // first line(s) might be empty
        if (currLine.size() != 0 && visitedBlock == index) {
            // Block with specifed index found

            // startPos might be assigned in if-clause of last iteration
            // or if no first empty line(s) then start pos has to be assigned now
            if (std::get<0>(data) == SIZE_MAX) {
                std::get<0>(data) = 0;
            }
            break;
        }

        lastLine = currLine;
        lastPos = stream.tellg();
    }

    if (visitedBlock != index) {
        throw std::invalid_argument(throw_message(
            "Passed index is not valid (to high) for given file!"));
    }

    // block found, first line of block already in currLine
    std::get<2>(data) = 1; // Size of block now is 1

    // Save access to lastPos
    lastPos = stream.tellg();
}

// TODO Code Review - could be smaller, more modularized
void d19::FileLineHandler::findBlockEnd(
    std::ifstream &stream, std::tuple<size_t, size_t, size_t> &data,
    size_t &lastPos) {
    std::string currLine{""};
    // Count size of block (in lines) and assign blockEndPos
    while (std::getline(stream, currLine, delimiter).good()) {

        if (logger) {
            logger->log("size/end block: currLine: " + currLine);
        }

        // End of block reached
        if (currLine.size() == 0) {
            // End of Block reached and endPos assigned in if-clause of last iteration TODO

            // if block only consist of a single
            if (std::get<1>(data) == SIZE_MAX) {
                std::get<1>(data) = lastPos; //std::get<0>(ret);
            }

            break;
        }

        if (currLine.size() != 0) {
            // Increment size of block
            std::get<2>(data) += 1;

            // Assign blockEndPos, if next line is empty
            std::get<1>(data) = stream.tellg();
        }

        lastPos = stream.tellg();
    }

    // Last line is eof
    if (currLine.size() != 0 && stream.eof()) {
        // Increment size of block
        std::get<2>(data) += 1;

        // Assign blockEndPos, if next line is empty
        stream.seekg(0, std::ios_base::end);
        std::get<1>(data) = stream.tellg(); // lastPos
    }
}

std::tuple<size_t, size_t, size_t> d19::FileLineHandler::findBlock() {
    // Function should only be called to initialize object
    assert(blockStartPos == SIZE_MAX);
    assert(blockEndPos == SIZE_MAX);
    assert(blockSize == SIZE_MAX);

    // start, end, size
    std::tuple<size_t, size_t, size_t> ret{SIZE_MAX, SIZE_MAX, SIZE_MAX};

    std ::ifstream stream{filename, std::ios_base::in};

    if (!stream.good()) {
        throw std::runtime_error(
            throw_message("Error: " + std::string(strerror(errno))));
    }
    size_t lastPos{0};
    findBlockStart(stream, ret, lastPos);
    ////////////
    findBlockEnd(stream, ret, lastPos);

    assert(std::get<0>(ret) != SIZE_MAX);
    assert(std::get<1>(ret) != SIZE_MAX);
    assert(std::get<2>(ret) != SIZE_MAX);

    return ret;
}

////////////// ITERATOR IMPL //////////////

d19::FileLineHandler::iterator::iterator(const std::string &filename,
                                         BlockPos pos, char delimiter,
                                         size_t blockStartPos,
                                         size_t blockEndPos)
    : iter_stream{filename, std::ios::in}, iter_streamCurrPos{0},
      iter_fileHash{std::hash<std::string>{}(filename)},
      iter_delimiter{delimiter}, iter_blockStartPos{blockStartPos},
      iter_blockEndPos{blockEndPos} {

    MoveToPos(pos);
};

// prefix
d19::FileLineHandler::iterator &d19::FileLineHandler::iterator::operator--() {
    MoveLine(d19::FileLineHandler::iterator::LineDirection::prior);

    return *this;
};

// pre-increment
d19::FileLineHandler::iterator &d19::FileLineHandler::iterator::operator++() {

    MoveLine(d19::FileLineHandler::iterator::LineDirection::next);

    return *this;
};

// Post-increment not possible - as iterator not copyable
// Normal iterator would overload post-increment operator

bool d19::FileLineHandler::iterator::operator==(const iterator &other) const {
    return (iter_streamCurrPos == other.iter_streamCurrPos) &&
           (iter_fileHash == other.iter_fileHash);
};
bool d19::FileLineHandler::iterator::operator!=(const iterator &other) const {
    return !(*this == other);
};

std::string d19::FileLineHandler::iterator::operator*() {
    size_t currPos = iter_stream.tellg();
    std::string str{};
    std::getline(iter_stream, str, iter_delimiter);
    iter_stream.seekg(currPos);
    return str;
};

void d19::FileLineHandler::iterator::MoveToDelimiter(
    d19::FileLineHandler::iterator::LineDirection dir) {
    std::streamoff offset =
        (dir == d19::FileLineHandler::iterator::LineDirection::next) ? 1 : -1;

    char letter = iter_stream.peek();
    // Move along line - Prior: to start, Next: To End
    while (iter_stream.good() && letter != iter_delimiter) {
        iter_stream.seekg(offset, std::ios_base::cur);
        letter = iter_stream.peek();
    }
}

void d19::FileLineHandler::iterator::MoveLine(
    d19::FileLineHandler::iterator::LineDirection dir) {
    assert(!iter_stream.eof());

    std::streamoff offset =
        (dir == d19::FileLineHandler::iterator::LineDirection::next) ? 1 : -1;

    // Prior Movement - jump over endl of upper line
    if (dir == d19::FileLineHandler::iterator::LineDirection::prior &&
        iter_stream.tellg() != 0) {
        iter_stream.seekg(offset * 2, std::ios_base::cur);
    }

    MoveToDelimiter(dir);

    // Next Movement - jump over end of lower line
    if (dir == d19::FileLineHandler::iterator::LineDirection::next &&
        !iter_stream.eof()) {
        iter_stream.seekg(offset, std::ios_base::cur);
    } else {
        // Prior Movement - go one back, as delimiter search points to delimiter
        iter_stream.seekg(-offset, std::ios_base::cur);
    }

    // Update for comparison of two iterators
    iter_streamCurrPos = iter_stream.tellg();

    if (iter_stream.fail()) {
        throw std::runtime_error(throw_message("Stream failed!"));
    }
}

size_t getFileEndPos(std::ifstream &stream) {
    auto currPos = stream.tellg();
    stream.seekg(0, std::ios_base::end);
    auto ret = stream.tellg();
    stream.seekg(currPos, std::ios_base::beg);
    return ret;
}

void d19::FileLineHandler::iterator::MoveToPos(BlockPos dir) {
    switch (dir) {
    case d19::FileLineHandler::iterator::BlockPos::start:
        iter_stream.seekg(iter_blockStartPos, std::ios_base::beg);
        iter_streamCurrPos = iter_blockStartPos;
        break;
    case d19::FileLineHandler::iterator::BlockPos::end:

        // Fix error when data block goes until end of file
        if (iter_blockEndPos == getFileEndPos(iter_stream)) {
            iter_blockEndPos -= 1;
        }

        iter_stream.seekg(iter_blockEndPos, std::ios_base::beg);
        iter_streamCurrPos = iter_blockEndPos;
        break;
    default:
        throw std::invalid_argument(throw_message(
            "Switch case passed is not known, should be element of "
            "d19::FileLineHandler::iterator::BlockPos!"));
    }

    if (iter_stream.fail()) {
        throw std::runtime_error(throw_message("Stream failed!"));
    }
}