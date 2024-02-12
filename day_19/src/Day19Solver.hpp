#ifndef AOC_D19_SOLVER_H
#define AOC_D19_SOLVER_H

#include <PartCoordinator.hpp>
#include <ThreadCoordinator.hpp>
#include <Workflow.hpp>
#include <logging/ILog.hpp>
#include <measuring/ElapsedTimer.hpp>

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace d19 {

/**
 * @brief An object solving an AOC exercise for a single object in its lifetime.
 * 
 */
class AOCSolver {

    std::shared_ptr<ILog> logger;
    std::vector<d19::Workflow> workflows;
    std::vector<d19::MachinePart> parts;

    /**
     * @brief Parses data from an input file and constructs workflows and parts saved in @a workflows and @a parts
     * Initializes invariants.
     * 
     * @param filename [in] filename to parse data from
     * 
     */
    void parsePartsAndWorkflows(const std::string &filename);

    struct SolveData {
        std::shared_ptr<d19::PartCoordinator> coordinatorParts;
        // coordinator to initialize a WorkFlowThread with
        std::shared_ptr<d19::ThreadCoordinator> coordinatorThreads;
        // to store created threads
        std::vector<std::thread> threads;
    };

    /**
     * @brief Distributes the workflows between WorkflowThread objects 
     *        and starts a thread for each object
     */
    [[nodiscard]] SolveData workerCreate(size_t threadAmount);

    /**
     * @brief Distributes a subset of all workflows into @a return.first.
     * 
     * @param beginWorkflows Pointer to current undistributed workflow
     * @param amountThreads Amount of workers to distribute all workflows between
     * @return std::pair<std::vector<d19::Workflow>, std::vector<d19::Workflow>::iterator> 
     */
    std::pair<std::vector<d19::Workflow>, std::vector<d19::Workflow>::iterator>
    workerDistributeWorkflows(
        std::vector<d19::Workflow>::iterator beginWorkflows,
        const size_t amountThreads);

    /**
     * @brief Adds a subset all parts to @a worker
     * 
     * @param beginParts iterator pointing to current part
     * @param amountThreads amount of threads to distribute all parts between
     * @param worker current worker a subset of parts is added to
     * @return std::vector<d19::MachinePart>::iterator New Parts pointer for recurrent call of this function
     */
    std::vector<d19::MachinePart>::iterator
    workerDistributeParts(std::vector<d19::MachinePart>::iterator beginParts,
                          const size_t amountThreads,
                          std::shared_ptr<d19::WorkflowThread> worker);

    /**
     * @brief Holds business logic for thread coordination
     * 
     * @param data Moved Data
     * @param timer to track needed time
     */
    void workerRun(SolveData &data, ElapsedTimer *timer);

    /**
     * @brief Outputs logs for sorted MachineParts 
     * 
     * @param data 
     * @param timer 
     */
    void workerFormattedLogging(const SolveData &data, ElapsedTimer *timer);

  public:
    /**
     * @brief Construct a new AOCSolver object to initialize 
     * MachineParts and Workflows extracted from @a filename
     * 
     * @param logger for debugging purposes
     * @param filename to get workflows and machineparts from
     */
    AOCSolver(std::shared_ptr<d19::ILog> logger, const std::string &filename);

    /**
     * @brief Sorts all MachineParts between all Workflows.
     * Thereby, all parts end up being categorized to @a accepted or @a rejected.
     * Finally, this function accumalted the ratings of accepted parts and returns it. 
     * 
     * @param amountThreads Amount of Threads to perform sorting with
     * @param timer to measure time of sorting
     * @return unsigned accumlated ratings of accepted parts
     */
    [[nodiscard]] unsigned sort(size_t amountThreads, ElapsedTimer *timer);
};

// namespace d19

} // namespace d19

#endif // AOC_D19_SOLVER_H