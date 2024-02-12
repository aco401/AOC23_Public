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
        //logger = std::make_shared<d19::FileLogger>("test_partcoordinator_log.txt");
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

TEST_F(FixturePartCoordinator, registerWorker_negative) {

    // nullptr
    EXPECT_THROW(coordinatorPart->registerWorker(nullptr),
                 std::invalid_argument);
}

TEST_F(FixturePartCoordinator, registerWorker_positive) {

    std::vector<d19::Workflow> inFlowSender{
        parser->fromStringWorkflow("bmn{a>2630:gr,a>2504:R,A}")};

    auto sender = std::make_shared<d19::WorkflowThread>(
        inFlowSender, coordinatorPart, coordinatorThread);

    EXPECT_NO_THROW(coordinatorPart->registerWorker(sender));
}

TEST_F(FixturePartCoordinator, transferPart_sorted_accepted) {

    // Accepted

    coordinatorPart->transferPart(
        d19::MachinePartStatus::accepted,
        parser->fromStringMachinePart("{x=342,m=311,a=2631,s=1594}"));

    auto [beginA, endA] =
        coordinatorPart->getSortedParts(d19::MachinePartStatus::accepted);
    EXPECT_EQ(*beginA,
              parser->fromStringMachinePart("{x=342,m=311,a=2631,s=1594}"));

    auto [beginR, endR] =
        coordinatorPart->getSortedParts(d19::MachinePartStatus::rejected);
    EXPECT_EQ(beginR, endR);
}

TEST_F(FixturePartCoordinator, transferPart_sorted_rejected) {

    // Rejected

    coordinatorPart->transferPart(
        d19::MachinePartStatus::rejected,
        parser->fromStringMachinePart("{x=3542,m=3511,a=26531,s=15954}"));

    auto [beginR, endR] =
        coordinatorPart->getSortedParts(d19::MachinePartStatus::rejected);
    EXPECT_EQ(*beginR,
              parser->fromStringMachinePart("{x=3542,m=3511,a=26531,s=15954}"));

    auto [beginA, endA] =
        coordinatorPart->getSortedParts(d19::MachinePartStatus::accepted);
    EXPECT_EQ(beginA, endA);
}

TEST_F(FixturePartCoordinator, transferPart_otherWorkflowThread) {

    std::vector<d19::Workflow> inFlowReceiver{
        parser->fromStringWorkflow("gr{A}")};

    auto receiver = std::make_shared<d19::WorkflowThread>(
        inFlowReceiver, coordinatorPart, coordinatorThread);

    coordinatorPart->registerWorker(receiver);

    // TransferPart to registered workflow
    coordinatorPart->transferPart(
        "gr", parser->fromStringMachinePart("{x=342,m=311,a=2631,s=1594}"));

    EXPECT_EQ(coordinatorPart->getSizeSortedParts(), 0);
}

TEST_F(FixturePartCoordinator, getSizeSortedParts) {

    // Transfer many Parts to coordinator
    unsigned randomNum =
        static_cast<unsigned>((std::rand() * std::rand())) % 100;
    for (size_t i = 0; i < randomNum; ++i) {
        coordinatorPart->transferPart(
            d19::MachinePartStatus::accepted,
            parser->fromStringMachinePart("{x=342,m=311,a=2631,s=1594}"));
    }

    EXPECT_EQ(coordinatorPart->getSizeSortedParts(), randomNum);
}

TEST_F(FixturePartCoordinator, accumulateAcceptedRating) {

    // Accepted
    coordinatorPart->transferPart(
        d19::MachinePartStatus::accepted,
        parser->fromStringMachinePart("{x=342,m=311,a=2631,s=1594}"));

    coordinatorPart->transferPart(
        d19::MachinePartStatus::accepted,
        parser->fromStringMachinePart("{x=342,m=311,a=100,s=1594}"));

    EXPECT_EQ(coordinatorPart->accumulateAcceptedRating(), 7225);
}

TEST_F(FixturePartCoordinator, unregisterWorker_key) {

    std::vector<d19::Workflow> inFlowReceiver{
        parser->fromStringWorkflow("gr{A}")};

    auto receiver = std::make_shared<d19::WorkflowThread>(
        inFlowReceiver, coordinatorPart, coordinatorThread);

    coordinatorPart->registerWorker(receiver);

    // TransferPart to registered workflow
    coordinatorPart->transferPart(
        "gr", parser->fromStringMachinePart("{x=342,m=311,a=2631,s=1594}"));

    coordinatorPart->unregisterWorker("gr");

    // TransferPart to unregistered workflow
    EXPECT_THROW(
        coordinatorPart->transferPart(
            "gr", parser->fromStringMachinePart("{x=342,m=311,a=2631,s=1594}")),
        std::invalid_argument);
}

TEST_F(FixturePartCoordinator, unregisterWorker_shared_ptr) {

    std::vector<d19::Workflow> inFlowReceiver{
        parser->fromStringWorkflow("gr{A}")};

    auto receiver = std::make_shared<d19::WorkflowThread>(
        inFlowReceiver, coordinatorPart, coordinatorThread);

    coordinatorPart->registerWorker(receiver);

    // TransferPart to registered workflow
    coordinatorPart->transferPart(
        "gr", parser->fromStringMachinePart("{x=342,m=311,a=2631,s=1594}"));

    coordinatorPart->unregisterWorker(receiver);

    // TransferPart to unregistered workflow
    EXPECT_THROW(
        coordinatorPart->transferPart(
            "gr", parser->fromStringMachinePart("{x=342,m=311,a=2631,s=1594}")),
        std::invalid_argument);
}
