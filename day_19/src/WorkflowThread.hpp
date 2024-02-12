#ifndef AOC_D19_WORKFLOW_THREAD
#define AOC_D19_WORKFLOW_THREAD

#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include <ThreadsafeQueue.hpp>
#include <Workflow.hpp>

#include <PartCoordinator.hpp>
#include <ThreadCoordinator.hpp>

namespace d19 {

// Forward declaration because of circular dependencies
//class PartCoordinator; // Forward declaration and include is redundant

class WorkflowThread {
  private:
    using optional_status =
        std::optional<std::variant<MachinePartStatus, std::string>>;

    std::map<std::string, Workflow> workflows;
    ThreadsafeQueue<std::pair<optional_status, d19::MachinePart>> unsortedParts;

    // weak_ptr to break circular ownership, used in parent graphs
    std::weak_ptr<PartCoordinator> coordinatorParts;

    std::shared_ptr<ThreadCoordinator> coordinatorThread;

  private:
    void buildMap(const std::vector<Workflow> &inputWorkflows);
    void sortByWorkflows(MachinePart part);
    void sortByWorkflows(std::variant<MachinePartStatus, std::string> status,
                         MachinePart part);

  public:
    /**
     * @brief Construct a new Workflow Thread object
     * 
     * @param inputWorkflows Container of workflows to sort parts with
     * @param coordinatorParts Coordinator to distrubute parts to
     * @param coordinatorThread Coordinator to controll start/stop of the @a operator() member function
     * 
     * @exception std::invalid_argument nullptr passed to coordinator
     * @exception std::invalid_argument if inputWorkflows is empty
     */
    WorkflowThread(const std::vector<Workflow> &inputWorkflows,
                   std::weak_ptr<PartCoordinator> coordinatorParts,
                   std::shared_ptr<ThreadCoordinator> coordinatorThread);

    /**
     * @brief Thread Entry: Takes parts from queue, sorts them 
     * and, if needed, transfers sorted parts to the PartCoordinator.
     * 
     * Starting and stopping is controlled by @a coordinatorThread 
     * 
     */
    void operator()();

    /**
     * @brief Add a MachinePart to the Worker Queue. Part is not pre-sorted.
     * 
     * @param part to add
     */
    void addPart(MachinePart part);

    /**
     * @brief Add a MachinePart to the Worker Queue. Part is pre-sorted.
     * This function will most likely be used, if another worker already sorted the part once, 
     * and this is the destination.
     * 
     * @param status pre-sorted status
     * @param part to add
     */
    void addPart(std::variant<MachinePartStatus, std::string> status,
                 MachinePart part);

    /**
     * @brief Return a container of all keys the WorkflowThread has Workflows for
     * 
     * @param keys [in/out] list of keys
     */
    void exportWorkflowKeys(std::vector<std::string> &keys);
};

} // namespace d19

#endif // AOC_D19_WORKFLOW_THREAD