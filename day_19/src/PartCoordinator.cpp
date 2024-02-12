#include <TraceException.hpp>
#include <algorithm>
#include <cassert>
#include <exception>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include <PartCoordinator.hpp>
#include <Utility.hpp>
#include <Workflow.hpp>
#include <WorkflowThread.hpp>

#include <ranges>

void d19::PartCoordinator::transferPart(
    std::variant<d19::MachinePartStatus, std::string> status,
    d19::MachinePart part) {
    if (std::holds_alternative<d19::MachinePartStatus>(status)) {
        {
            const std::lock_guard uLock{partsAccess};
            switch (std::get<d19::MachinePartStatus>(status)) {
            case d19::MachinePartStatus::accepted:
                accepted.push_back(part);

                break;
            case d19::MachinePartStatus::rejected:
                rejected.push_back(part);

                break;
            default:
                throw std::invalid_argument(throw_message(
                    "Switch case passed is not known, should be element of "
                    "d19::MachinePartStatus!"));
            }
        }
        sortedParts += 1;
        sortedCond.notify_one();
    } else {

        const std::lock_guard uLock{workerAccess};

        if (!workflowToThread.contains(std::get<std::string>(status))) {
            throw std::invalid_argument(
                throw_message("WorkflowKey in passed argument is not known!"));
        }

        const unsigned workerIndex =
            workflowToThread.at(std::get<std::string>(status));
        workers.at(workerIndex)->addPart(status, part);
    }
};

std::pair<std::vector<d19::MachinePart>::const_iterator,
          std::vector<d19::MachinePart>::const_iterator>
d19::PartCoordinator::getSortedParts(d19::MachinePartStatus type) {
    if (!workers.empty()) {
        throw std::domain_error(throw_message(
            "Precondition is not met. Unprotected Shared storage "
            "might be changed during read. Workers have to stop work."));
    }

    switch (type) {
    case d19::MachinePartStatus::accepted:
        return std::make_pair(accepted.begin(), accepted.end());
        break;
    case d19::MachinePartStatus::rejected:
        return std::make_pair(rejected.begin(), rejected.end());
        break;
    default:
        throw std::invalid_argument(throw_message(
            "Switch case passed is not known, should be element of "
            "d19::MachinePartStatus!"));
    }
};

void d19::PartCoordinator::registerWorker(
    std::shared_ptr<d19::WorkflowThread> worker) {

    if (!worker) {
        throw std::invalid_argument(
            throw_message("Nullptr to parameter worker passed!"));
    }

    std::vector<std::string> workflowKeys{};
    worker->exportWorkflowKeys(workflowKeys);

    if (workflowKeys.empty()) {
        throw std::invalid_argument(throw_message("Container is empty!"));
    }

    if (std::any_of(workflowKeys.begin(), workflowKeys.end(),
                    [](const auto &elem) { return elem.empty(); })) {
        throw std::invalid_argument(
            throw_message("An element of the passed container is empty!"));
    }

    std::lock_guard uLock{workerAccess};

    unsigned unusedKey =
        d19::findUnusedKey<std::shared_ptr<WorkflowThread>>(workers);

    workers.insert({unusedKey, worker});

    for (const auto &elem : workflowKeys) {
        workflowToThread.insert(std::make_pair(elem, unusedKey));
    }
};

void d19::PartCoordinator::findWorkerAndRemove(
    std::shared_ptr<d19::WorkflowThread> workerRemove) {
    unsigned index{0};
    for (auto it = workers.begin(); it != workers.end(); ++it) {

        if (it->second == workerRemove) {
            index = it->first;
            workers.erase(it);
            break;
        }

        if (it == (--workers.end())) {
            throw std::invalid_argument(throw_message(
                "Passed Worker was not registered with Coordinator!"));
        }
    }

    for (auto it = workflowToThread.begin(); it != workflowToThread.end();) {
        if (it->second == index) {
            it = workflowToThread.erase(it);
        } else {
            ++it;
        }
    }
}

void d19::PartCoordinator::unregisterWorker(
    std::shared_ptr<d19::WorkflowThread> workerRemove) {

    if (!workerRemove) {
        throw std::invalid_argument(
            throw_message("Nullptr to parameter worker passed!"));
    }

    std::lock_guard uLock{workerAccess};
    findWorkerAndRemove(workerRemove);
};

void d19::PartCoordinator::unregisterWorker(const std::string &workflowKey) {

    std::lock_guard uLock{workerAccess};
    if (!workflowToThread.contains(workflowKey)) {
        throw std::invalid_argument(
            throw_message("Passed Key is not mapped with any worker!"));
    }

    if (workflowKey.empty()) {
        throw std::invalid_argument(throw_message("Passed Key is empty!"));
    }

    findWorkerAndRemove(workers.at(workflowToThread.at(workflowKey)));
};

unsigned d19::PartCoordinator::getSizeSortedParts() const {
    return sortedParts;
}

void d19::PartCoordinator::waitForSorted(unsigned size) {
    std::unique_lock tmp{partsAccess};

    sortedCond.wait(tmp, [this, size] { return size == sortedParts; });
}

unsigned d19::PartCoordinator::accumulateAcceptedRating() {

    unsigned rating{0};
    std::for_each(
        accepted.begin(), accepted.end(),
        [&rating](const d19::MachinePart &elem) { rating += elem.sum(); });
    return rating;
}