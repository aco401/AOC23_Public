#include <algorithm>
#include <iostream>
#include <memory>

#include <cctype>
#include <cstddef>
#include <getopt.h>
#include <string>
#include <string_view>

#include <logging/CoutLogger.hpp>
#include <logging/DummyLogger.hpp>
#include <logging/FileLogger.hpp>
#include <measuring/ElapsedTimer.hpp>

#include <ILog.hpp>

#include <Day19Solver.hpp>

const char *FILE_DATA_EXAMPLE{"../src/data_example.txt"};

const char *FILE_DATA_INPUT{"../src/data_input.txt"};

const char *LOG_FILE{"log.txt"};

bool isNumber(std::string_view str) {
    return std::all_of(str.begin(), str.end(),
                       [](unsigned char c) { return std::isdigit(c); });
}

void abortOnNotNumber(std::string_view str) {
    if (!isNumber(str)) {
        std::cout << "Argument passed to parameter is not a number"
                  << std::endl;
        abort();
    }
}

int main(int argc, char **argv) {

    // ----------- CLI Parameter Parsing  --------------

    size_t amount_threads = 1;
    const char *filename = FILE_DATA_INPUT;
    std::shared_ptr<d19::ILog> logger{std::make_shared<d19::DummyLogger>()};
    //std::make_shared<d19::FileLogger>(LOG_FILE);
    //std::make_shared<d19::CoutLogger>();

    option longopts[] = {{"threads", required_argument, NULL, 't'},
                         {"file", optional_argument, NULL, 'f'},
                         {"logger", no_argument, NULL, 'l'},
                         {0, 0, 0, 0}};

    while (1) {
        const int opt = getopt_long(argc, argv, "t:f:", longopts, 0);

        if (opt == -1) {
            break;
        }

        switch (opt) {

        case 't':
            abortOnNotNumber(optarg);
            std::cout << "option -t with arg " << optarg << "\n";
            amount_threads = std::stol(optarg);
            break;
        case 'f':
            std::cout << "option -f with arg " << optarg << "\n";
            filename = optarg;
            break;
        case 'l':
            std::cout << "option -l\n";
            logger = std::make_shared<d19::CoutLogger>();
            break;
        }
    }

    std::cout << "Threads to run: " << std::to_string(amount_threads)
              << std::endl;

    // -------------- Application specific Code  -------------------

    d19::ElapsedTimer timer{};

    d19::AOCSolver solver(logger, filename);
    const unsigned TotalRating = solver.sort(amount_threads, &timer);
    std::cout << "Sorting time in ms: " << timer.getElapsedTime() << "\n";
    std::cout << "Rating of accepted Parts: " << std::to_string(TotalRating)
              << "\n";
    return 0;
}