#include <iostream>

#include <Day5Solver.hpp>

#include <FileLogger.hpp>

constexpr const char *file_data_example = "../src/data_example.txt";
constexpr const char *file_data_input = "../src/data_input.txt";

constexpr const char *log_file = "log.txt";

int main(int, char **) {
    std::shared_ptr<ns_day_5::ILog> logger =
        std::make_shared<ns_day_5::FileLogger>(std::move(log_file));

    std::pair<int, int> solution = ns_day_5::solve_example(logger);
    std::cout << "Example: The seed " + std::to_string(solution.first) +
                     " has the lowest location of " +
                     std::to_string(solution.second)
              << "\n";

    solution = ns_day_5::solve_file(logger, file_data_example);
    std::cout << "File " + std::string(file_data_example) + ": The seed " +
                     std::to_string(solution.first) +
                     " has the lowest location of " +
                     std::to_string(solution.second)
              << "\n";

    solution = ns_day_5::solve_file(logger, file_data_input);
    std::cout << "File " + std::string(file_data_input) + ": The seed " +
                     std::to_string(solution.first) +
                     " has the lowest location of " +
                     std::to_string(solution.second)
              << "\n";
    return 0;
}
