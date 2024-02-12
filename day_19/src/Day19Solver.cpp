#include <Day19Solver.hpp>
#include <PartCoordinator.hpp>
#include <ThreadCoordinator.hpp>
#include <Workflow.hpp>
#include <WorkflowStringParser.hpp>
#include <WorkflowThread.hpp>
#include <filehandling/FileLineHandler.hpp>
#include <logging/ILog.hpp>
#include <measuring/ElapsedTimer.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>

d19::AOCSolver::AOCSolver(std::shared_ptr<d19::ILog> logger,
                          const std::string &filename) {
    if (filename.empty()) {
        throw std::invalid_argument("Passed filename is empty!");
    }

    if (logger) {
        logger->log("Solving File: " + filename);
    }

    parsePartsAndWorkflows(filename);

    if (workflows.empty() || parts.empty()) {
        throw std::invalid_argument("Not enough data parsed to work on!");
    }

    if (logger) {
        logger->log("Size: Workflows: " + std::to_string(workflows.size()));
        logger->log("Size: Parts: " + std::to_string(parts.size()));
    }
}

unsigned d19::AOCSolver::sort(size_t amountThreads, ElapsedTimer *timer) {

    if (amountThreads > workflows.size()) {
        throw std::invalid_argument(
            "The amount of threads is to big for the amount of workflows!");
    }

    if (logger) {
        logger->log("Size: Threads: " + std::to_string(amountThreads));
    }

    SolveData data = workerCreate(amountThreads);

    workerRun(data, timer);

    workerFormattedLogging(data, timer);

    return data.coordinatorParts->accumulateAcceptedRating();
};

void d19::AOCSolver::parsePartsAndWorkflows(const std::string &filename) {

    d19::WorkflowStringParser parser{};
    d19::FileLineHandler file_wf{logger, filename, '\n', 0};
    d19::FileLineHandler file_mp{logger, filename, '\n', 1};

    for (auto it = file_wf.begin(); it != file_wf.end(); ++it) {
        workflows.push_back(parser.fromStringWorkflow(*it));
    }

    for (auto it = file_mp.begin(); it != file_mp.end(); ++it) {
        parts.push_back(parser.fromStringMachinePart(*it));
    }
};

std::pair<std::vector<d19::Workflow>, std::vector<d19::Workflow>::iterator>
d19::AOCSolver::workerDistributeWorkflows(
    std::vector<d19::Workflow>::iterator beginWorkflows,
    const size_t amountThreads) {

    std::vector<d19::Workflow> subsetWorkflows{};
    const size_t workflowsPerWorker{workflows.size() /
                                    amountThreads}; // wanted conversion loss!

    // Pass Workflows
    beginWorkflows = std::for_each_n(
        beginWorkflows, workflowsPerWorker,
        [&subsetWorkflows](auto &elem) { subsetWorkflows.push_back(elem); });

    if (auto dist = std::distance(beginWorkflows, workflows.end());
        dist > 0 && dist == (workflows.size() % amountThreads)) {
        beginWorkflows = std::for_each_n(beginWorkflows, dist,
                                         [&subsetWorkflows](auto &elem) {
                                             subsetWorkflows.push_back(elem);
                                         });
    }

    return std::make_pair(subsetWorkflows, beginWorkflows);
}

std::vector<d19::MachinePart>::iterator d19::AOCSolver::workerDistributeParts(
    std::vector<d19::MachinePart>::iterator beginParts,
    const size_t amountThreads, std::shared_ptr<d19::WorkflowThread> worker) {

    // Init Parts
    const size_t partsPerWorker{parts.size() /
                                amountThreads}; // wanted conversion loss!

    // Pass MachineParts
    beginParts =
        std::for_each_n(beginParts, partsPerWorker,
                        [&worker](auto &elem) { worker->addPart("in", elem); });

    if (auto dist = std::distance(beginParts, parts.end());
        dist > 0 && dist == (parts.size() % amountThreads)) {
        beginParts = std::for_each_n(beginParts, dist, [&worker](auto &elem) {
            worker->addPart("in", elem);
        });
    }

    return beginParts;
}

d19::AOCSolver::SolveData
d19::AOCSolver::workerCreate(const size_t threadAmount) {

    SolveData data{std::make_shared<d19::PartCoordinator>(),
                   std::make_shared<d19::ThreadCoordinator>(),
                   std::vector<std::thread>()};

    std::vector<d19::Workflow>::iterator iterWorkflows = workflows.begin();
    std::vector<d19::MachinePart>::iterator iterParts = parts.begin();

    for (size_t i = 0; i < threadAmount; ++i) {

        std::vector<d19::Workflow> subsetWorkflows{};
        std::tie(subsetWorkflows, iterWorkflows) =
            workerDistributeWorkflows(iterWorkflows, threadAmount);

        // Needs to be a shared_ptr, as thread and the coordinator needs to access the objects
        std::shared_ptr<d19::WorkflowThread> wft =
            std::make_shared<d19::WorkflowThread>(subsetWorkflows,
                                                  data.coordinatorParts,
                                                  data.coordinatorThreads);

        iterParts = workerDistributeParts(iterParts, threadAmount, wft);

        data.coordinatorParts->registerWorker(wft);

        data.threads.emplace_back([wft] { wft->operator()(); });
    }

    assert(data.threads.size() == threadAmount);
    assert(iterWorkflows == workflows.end());
    assert(iterParts == parts.end());

    return std::move(data);
};

void d19::AOCSolver::workerRun(SolveData &data, ElapsedTimer *timer) {
    if (timer) {
        timer->start();
    }

    // Start sorting by releasing lock
    data.coordinatorThreads->releaseStart();

    data.coordinatorParts->waitForSorted(parts.size());

    if (timer) {
        timer->stop();
    }

    // To stop while-loops of threads
    data.coordinatorThreads->requestStop();

    // Might be unnecesarry with jthreads
    for (auto &t : data.threads) {
        t.join();
    }
}

void d19::AOCSolver::workerFormattedLogging(const SolveData &data,
                                            ElapsedTimer *timer) {
    // ------------ Cout Output after computation -------------
    if (logger) {
        if (timer) {
            logger->log("Time for computation: " +
                        std::to_string(timer->getElapsedTime().count()) + "ms");
        }

        logger->log(
            "Sorted Parts: " +
            std::to_string(data.coordinatorParts->getSizeSortedParts()));

        {
            auto [begin, end] = data.coordinatorParts->getSortedParts(
                d19::MachinePartStatus::accepted);
            logger->log("Accepted Parts: " +
                        std::to_string(std::count_if(
                            begin, end, [](auto) { return true; })));
        }

        {
            auto [begin, end] = data.coordinatorParts->getSortedParts(
                d19::MachinePartStatus::rejected);
            logger->log("Rejected Parts: " +
                        std::to_string(std::count_if(
                            begin, end, [](auto) { return true; })));
        }
    }
}
