#include <WorkflowThread.hpp>
#include <gtest/gtest.h>

#include <WorkflowStringParser.hpp>
#include <logging/DummyLogger.hpp>
#include <logging/FileLogger.hpp>

#include <stop_token>

using namespace d19;

class FixtureWorkflowThread : public ::testing::Test {
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

TEST_F(FixtureWorkflowThread, constructor_Positive) {

    std::vector<d19::Workflow> inFlows{
        parser->fromStringWorkflow("bmn{a>2630:gr,a>2504:R,A}")};

    EXPECT_NO_THROW(
        d19::WorkflowThread(inFlows, coordinatorPart, coordinatorThread));
}

TEST_F(FixtureWorkflowThread, Constructor_negative) {
    std::shared_ptr<d19::WorkflowThread> sender = nullptr;

    // First Empty Vector
    std::vector<d19::Workflow> inFlowsEmpty{};
    EXPECT_THROW(
        d19::WorkflowThread(inFlowsEmpty, coordinatorPart, coordinatorThread),
        std::invalid_argument);

    // Second Nullptr
    std::vector<d19::Workflow> inFlows{
        parser->fromStringWorkflow("bmn{a>2630:gr,a>2504:R,A}")};
    EXPECT_THROW(d19::WorkflowThread(inFlows,
                                     std::weak_ptr<d19::PartCoordinator>(),
                                     coordinatorThread),
                 std::invalid_argument);

    // Third Nullptr
    EXPECT_THROW(d19::WorkflowThread(inFlows, coordinatorPart, nullptr),
                 std::invalid_argument);
}

TEST_F(FixtureWorkflowThread, Constructor_positiv) {

    std::vector<d19::Workflow> inFlows{
        parser->fromStringWorkflow("bmn{a>2630:gr,a>2504:R,A}")};
    EXPECT_NO_THROW(
        d19::WorkflowThread(inFlows, coordinatorPart, coordinatorThread));
}

TEST_F(FixtureWorkflowThread, addPart_MP_positive) {

    std::vector<d19::Workflow> inFlows{
        parser->fromStringWorkflow("bmn{a>2630:gr,a>2504:R,A}")};
    d19::WorkflowThread wft(inFlows, coordinatorPart, coordinatorThread);

    EXPECT_NO_THROW(wft.addPart(
        parser->fromStringMachinePart("{x=342,m=311,a=2631,s=1594}")));
}

TEST_F(FixtureWorkflowThread, addPart_MP_S_negative) {

    std::vector<d19::Workflow> inFlows{
        parser->fromStringWorkflow("bmn{a>2630:gr,a>2504:R,A}")};
    d19::WorkflowThread wft(inFlows, coordinatorPart, coordinatorThread);

    std::variant<MachinePartStatus, std::string> status{""};

    EXPECT_THROW(wft.addPart(status, parser->fromStringMachinePart(
                                         "{x=342,m=311,a=2631,s=1594}")),
                 std::invalid_argument);
}

TEST_F(FixtureWorkflowThread, addPart_MP_S_positive) {

    std::vector<d19::Workflow> inFlows{
        parser->fromStringWorkflow("bmn{a>2630:gr,a>2504:R,A}")};
    d19::WorkflowThread wft(inFlows, coordinatorPart, coordinatorThread);

    std::variant<MachinePartStatus, std::string> status{"bladwq"};

    EXPECT_NO_THROW(wft.addPart(
        status, parser->fromStringMachinePart("{x=342,m=311,a=2631,s=1594}")));

    status = d19::MachinePartStatus::accepted;

    EXPECT_NO_THROW(wft.addPart(
        status, parser->fromStringMachinePart("{x=342,m=311,a=2631,s=1594}")));

    status = d19::MachinePartStatus::rejected;

    EXPECT_NO_THROW(wft.addPart(
        status, parser->fromStringMachinePart("{x=342,m=311,a=2631,s=1594}")));
}

TEST_F(FixtureWorkflowThread, exportWorkFlowKeys) {

    std::vector<d19::Workflow> inFlows{
        parser->fromStringWorkflow("bmn{a>2630:gr,a>2504:R,A}"),
        parser->fromStringWorkflow("bla{a>2630:gr,a>2504:R,A}"),
        parser->fromStringWorkflow("hi{a>2630:gr,a>2504:R,A}"),
        parser->fromStringWorkflow("hey{a>2630:gr,a>2504:R,A}"),
        parser->fromStringWorkflow("test{a>2630:gr,a>2504:R,A}")};
    d19::WorkflowThread wft(inFlows, coordinatorPart, coordinatorThread);

    std::vector<std::string> keys{};
    wft.exportWorkflowKeys(keys);

    EXPECT_EQ(keys.size(), 5);

    EXPECT_TRUE(std::find(keys.begin(), keys.end(), "bmn") != keys.end());
    EXPECT_TRUE(std::find(keys.begin(), keys.end(), "bla") != keys.end());
    EXPECT_TRUE(std::find(keys.begin(), keys.end(), "hi") != keys.end());
    EXPECT_TRUE(std::find(keys.begin(), keys.end(), "hey") != keys.end());
    EXPECT_TRUE(std::find(keys.begin(), keys.end(), "test") != keys.end());
}
