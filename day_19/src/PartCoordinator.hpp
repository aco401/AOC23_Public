#ifndef AOC_D19_PART_COORDINATOR
#define AOC_D19_PART_COORDINATOR

#include <condition_variable>
#include <cstdint>
#include <map>
#include <memory>

#include <string>
#include <variant>
#include <vector>

#include <Workflow.hpp>

namespace d19 {

// Forward declaration because of circular dependencies
class WorkflowThread;

class PartCoordinator {
  private:
    std::mutex workerAccess;
    std::map<std::string, unsigned> workflowToThread;
    std::map<unsigned, std::shared_ptr<WorkflowThread>> workers;

    std::mutex partsAccess;
    std::vector<MachinePart> accepted;
    std::vector<MachinePart> rejected;
    std::condition_variable sortedCond;

    std::atomic_uint32_t sortedParts;

    void findWorkerAndRemove(std::shared_ptr<WorkflowThread> workerRemove);

  public:
    PartCoordinator() = default;
    // I Cant think of a scenario where i need to copy a PartCoordinator
    PartCoordinator(PartCoordinator const &) = delete;
    PartCoordinator &operator=(PartCoordinator const &) = delete;

    // There might be a scenario where a move is needed - not in this project though
    PartCoordinator(PartCoordinator &&) = delete;
    PartCoordinator &operator=(PartCoordinator &&) = delete;
    ~PartCoordinator() = default;

    /**
     * @brief Transfer MachinePart to other workflows or put it into accepted/rejected storage
     * 
     * @param status [in] resource to decide on 
     * @param part [in] Machine Part to transfer
     * 
     * @exception std::invalid_argument if a route to the passed workflow key in @a status is not known
     * 
     */
    void transferPart(std::variant<MachinePartStatus, std::string> status,
                      MachinePart part);

    /**
     * @brief Gets a start and end iterator for sorted parts traversal
     * 
     * @param type type of parts for traversal
     * @return std::pair<std::vector<MachinePart>::const_iterator,
     * std::vector<MachinePart>::const_iterator>
     * 
     * @warning Access to shared mutual state - iterators are not secured by mutex
     * @exception std::invalid_argument if coordinator still has registered workers
     */
    [[nodiscard]] std::pair<std::vector<MachinePart>::const_iterator,
                            std::vector<MachinePart>::const_iterator>
    getSortedParts(MachinePartStatus type);

    /**
     * @brief Get the amount of sorted parts
     * 
     * @return unsigned sorted parts
     */
    [[nodiscard]] unsigned getSizeSortedParts() const;

    /**
     * @brief Blocks as long as the amount of sorted parts is equal to @a size 
     * 
     * @param size Amount of sorted parts to wait for
     */
    void waitForSorted(unsigned size);

    /**
     * @brief Registers a WorkflowThread @a worker and tracks its responsibility to a set of workflows.
     * 
     * @param worker shared_ptr to the worker
     * 
     * @exception std::invalid_argument Nullptr passed to @a worker
     * @exception std::invalid_argument @a workflowKeys is empty
     */
    void registerWorker(std::shared_ptr<WorkflowThread> worker);

    /**
     * @brief Deletes the connection between a WorkflowThread and its responsible workflows.
     * 
     * @param worker shared_ptr to the worker
     * 
     * @exception std::invalid_argument Nullptr passed to @a worker
     * @exception std::invalid_argument @a workflowKeys is empty
     */
    void unregisterWorker(std::shared_ptr<WorkflowThread> worker);

    /**
     * @brief Deletes the connection between a WorkflowThread 
     * and its responsible workflows by passing a single key.
     * 
     * @param workflowKey deletable keys 
     * 
     * @exception std::invalid_argument Nullptr passed to @a worker
     * @exception std::invalid_argument @a workflowKeys is empty
     */
    void unregisterWorker(const std::string &workflowKey);

    /**
     * @brief Accumulates the ratings of all accepted parts
     * 
     * @return unsigned accumlated ratings
     */
    [[nodiscard]] unsigned accumulateAcceptedRating();
};

} // namespace d19

#endif // AOC_D19_PART_COORDINATOR