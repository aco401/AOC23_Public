#include <TraceException.hpp>

#include <memory>
#include <optional>
#include <stdexcept>
#include <stop_token>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include <PartCoordinator.hpp>
#include <ThreadCoordinator.hpp>
#include <Workflow.hpp>
#include <WorkflowThread.hpp>

constexpr unsigned WAIT_ON_NOT_EMPTY_MS = 100;

d19::WorkflowThread::WorkflowThread(
    const std::vector<d19::Workflow> &inputWorkflows,
    std::weak_ptr<PartCoordinator> coordinatorParts,
    std::shared_ptr<ThreadCoordinator> coordinatorThread)
    : workflows{}, unsortedParts{}, coordinatorParts{coordinatorParts},
      coordinatorThread{coordinatorThread} {

    if (this->coordinatorParts.expired() || !coordinatorThread) {
        throw std::invalid_argument(
            throw_message("Nullptr to parameter passed!"));
    }

    if (inputWorkflows.empty()) {
        throw std::invalid_argument(throw_message("Container is empty()!"));
    }

    buildMap(inputWorkflows);
};

void d19::WorkflowThread::buildMap(
    const std::vector<d19::Workflow> &inputWorkflows) {

    for_each(inputWorkflows.begin(), inputWorkflows.end(),
             [this](const d19::Workflow &elem) {
                 workflows.insert({elem.getKey(), elem});
             });
};

void d19::WorkflowThread::operator()() {
    // Should wait until start is notified
    coordinatorThread->waitToStart();

    std::stop_token stop = coordinatorThread->getStopResource();

    while (!stop.stop_requested()) {
        unsortedParts.waitOnNotEmpty(
            std::chrono::milliseconds(WAIT_ON_NOT_EMPTY_MS));

        if (!unsortedParts.empty()) {
            auto part = unsortedParts.pop();
            if (part.first.has_value()) {
                sortByWorkflows(part.first.value(), part.second);
            } else {
                sortByWorkflows(part.second);
            }
        }
    }

    if (auto parts = coordinatorParts.lock(); parts != nullptr) {
        parts->unregisterWorker(workflows.begin()->first);
    } else {
        throw std::runtime_error(
            throw_message("PartCoordinator lifetime ended!"));
    }
};

void d19::WorkflowThread::sortByWorkflows(
    std::variant<d19::MachinePartStatus, std::string> status,
    d19::MachinePart part) {

    while (true) {

        // Look whether workflow key is found in local workflows
        if (std::holds_alternative<std::string>(status) &&
            workflows.contains(std::get<std::string>(status))) {
            auto iter = workflows.find(std::get<std::string>(status));
            status = iter->second.sortMachinePart(part);
        } else {
            // key is not found in local workflows or part is finally sorted to accepted/rejected
            if (auto parts = coordinatorParts.lock(); parts != nullptr) {
                parts->transferPart(status, part);
            } else {
                throw std::runtime_error(
                    throw_message("PartCoordinator lifetime ended!"));
            }
            break;
        }
    }
}

void d19::WorkflowThread::sortByWorkflows(d19::MachinePart part) {
    // Just take first workflow
    auto eval = workflows.begin()->second.sortMachinePart(part);

    sortByWorkflows(eval, part);
}

void d19::WorkflowThread::addPart(d19::MachinePart part) {
    unsortedParts.push(std::make_pair(std::nullopt, part));
};

void d19::WorkflowThread::addPart(
    std::variant<d19::MachinePartStatus, std::string> status,
    MachinePart part) {

    if (std::holds_alternative<std::string>(status) &&
        std::get<std::string>(status).empty()) {

        throw std::invalid_argument(
            throw_message("String of parameter status is empty!"));
    }

    unsortedParts.push(std::make_pair(status, part));
}

void d19::WorkflowThread::exportWorkflowKeys(std::vector<std::string> &keys) {
    std::for_each(workflows.begin(), workflows.end(),
                  [&keys](auto elem) { keys.push_back(elem.first); });
}
