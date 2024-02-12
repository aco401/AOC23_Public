#include <gtest/gtest.h>

#include <logging/DummyLogger.hpp>
#include <logging/FileLogger.hpp>

#include <PartCoordinator.hpp>
#include <ThreadCoordinator.hpp>
#include <WorkflowStringParser.hpp>
#include <WorkflowThread.hpp>

#include <cstdlib>

using namespace d19;

class FixturePartCoordinator : public ::testing::Test {
  protected:
    void SetUp(void) {
        logger = std::make_shared<d19::DummyLogger>();
        coordinatorPart = std::make_shared<d19::PartCoordinator>();
        coordinatorThread = std::make_shared<d19::ThreadCoordinator>();
        parser = std::make_unique<d19::WorkflowStringParser>(logger);
    }

    void TearDown(void) {}

    std::shared_ptr<d19::ILog> logger;
    std::shared_ptr<d19::PartCoordinator> coordinatorPart;
    std::shared_ptr<d19::ThreadCoordinator> coordinatorThread;
    std::unique_ptr<d19::WorkflowStringParser> parser;
};

TEST_F(FixturePartCoordinator,
       registerWorker_waitforSorted_addPart_integration_test) {

    std::vector<std::thread> threads{};

    std::vector<d19::MachinePart> parts = {
        parser->fromStringMachinePart("{x=342,m=311,a=2631,s=1594}")};

    std::vector<d19::Workflow> inFlowSender{
        parser->fromStringWorkflow("bmn{a>2630:gr,a>2504:R,R}")};

    auto sender = std::make_shared<d19::WorkflowThread>(
        inFlowSender, coordinatorPart, coordinatorThread);

    sender->addPart(parts.at(0));

    threads.emplace_back([sender] { sender->operator()(); });

    coordinatorPart->registerWorker(sender);

    std::vector<d19::Workflow> inFlowReceiver{
        parser->fromStringWorkflow("gr{A}")};

    auto receiver = std::make_shared<d19::WorkflowThread>(
        inFlowReceiver, coordinatorPart, coordinatorThread);

    coordinatorPart->registerWorker(receiver);

    threads.emplace_back([receiver] { receiver->operator()(); });

    // Start sorting by releasing lock
    coordinatorThread->releaseStart();

    coordinatorPart->waitForSorted(1);

    // To stop while-loops of threads
    coordinatorThread->requestStop();

    // Might be unnecesarry with jthreads
    for (auto &t : threads) {
        t.join();
    }

    //
    EXPECT_EQ(coordinatorPart->getSizeSortedParts(), 1);

    // Has to be transferred to both workflowThread,
    // as only the receiver has an rule that point to accepted
    auto [begin, end] =
        coordinatorPart->getSortedParts(d19::MachinePartStatus::accepted);
    EXPECT_EQ((*begin).to_string(), "{x=342,m=311,a=2631,s=1594}");
}