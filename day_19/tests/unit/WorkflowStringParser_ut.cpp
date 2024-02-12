#include <gtest/gtest.h>

#include <logging/DummyLogger.hpp>
#include <logging/FileLogger.hpp>
#include <logging/ILog.hpp>

#include <WorkflowStringParser.hpp>
#include <variant>

#include <memory>
#include <regex>

using namespace d19;

class FixtureWorkflowStringParser : public ::testing::Test {
  protected:
    void SetUp(void) {
        //logger = std::make_shared<d19::FileLogger>("Test_FileLineHandler_log.txt");
        logger = std::make_shared<d19::DummyLogger>();
        parser = std::make_unique<d19::WorkflowStringParser>(logger);
    }

    void TearDown(void) {}

    std::shared_ptr<d19::ILog> logger;
    std::unique_ptr<d19::WorkflowStringParser> parser;
};

TEST_F(FixtureWorkflowStringParser, Machinepart_regex_positive) {

    std::regex reg{
        parser->getRegex(d19::WorkflowStringParser::ParsedTypes::machinepart)};

    std::string part{"{x=342,m=311,a=770,s=1594}"};

    EXPECT_TRUE(std::regex_match(part, reg));
}

TEST_F(FixtureWorkflowStringParser, Machinepart_regex_negative) {

    std::regex reg{
        parser->getRegex(d19::WorkflowStringParser::ParsedTypes::machinepart)};

    // One key missing
    EXPECT_FALSE(std::regex_match("{x=342,m=311,a=770}", reg));

    // Braces missing
    EXPECT_FALSE(std::regex_match("x=342,m=311,a=770,s=221", reg));

    // key and equal missing
    EXPECT_FALSE(std::regex_match("{x=342,m=311,770,s=221}", reg));

    // equal missing
    EXPECT_FALSE(std::regex_match("{x=342,m311,a=770,s=221}", reg));
}

TEST_F(FixtureWorkflowStringParser, RuleComparator_regex_positive) {

    std::regex reg{parser->getRegex(
        d19::WorkflowStringParser::ParsedTypes::rulecomparator)};

    EXPECT_TRUE(std::regex_match("x>2734", reg));
    EXPECT_TRUE(std::regex_match("m>6", reg));
    EXPECT_TRUE(std::regex_match("a>5000", reg));
    EXPECT_TRUE(std::regex_match("s>100", reg));

    EXPECT_TRUE(std::regex_match("x<1", reg));
    EXPECT_TRUE(std::regex_match("m<987", reg));
    EXPECT_TRUE(std::regex_match("a<440", reg));
    EXPECT_TRUE(std::regex_match("s<1234", reg));
}

TEST_F(FixtureWorkflowStringParser, RuleComparator_regex_negative) {

    std::regex reg{parser->getRegex(
        d19::WorkflowStringParser::ParsedTypes::rulecomparator)};

    // Other key
    EXPECT_FALSE(std::regex_match("p<1234", reg));

    // Other operation
    EXPECT_FALSE(std::regex_match("a=1234", reg));

    // negative number
    EXPECT_FALSE(std::regex_match("x>-1234", reg));
}

TEST_F(FixtureWorkflowStringParser, Rule_regex_positive) {

    std::regex reg{
        parser->getRegex(d19::WorkflowStringParser::ParsedTypes::rule)};

    EXPECT_TRUE(std::regex_match("x>2734:A", reg));
    EXPECT_TRUE(std::regex_match("m>6:R", reg));
    EXPECT_TRUE(std::regex_match("a>5000:gr", reg));
    EXPECT_TRUE(std::regex_match("s>100:af", reg));

    EXPECT_TRUE(std::regex_match("x<1:tr", reg));
    EXPECT_TRUE(std::regex_match("m<987:we", reg));
    EXPECT_TRUE(std::regex_match("a<440:R", reg));
    EXPECT_TRUE(std::regex_match("s<1234:A", reg));

    EXPECT_TRUE(std::regex_match("A", reg));
    EXPECT_TRUE(std::regex_match("R", reg));
    EXPECT_TRUE(std::regex_match("gr", reg));
}

TEST_F(FixtureWorkflowStringParser, Rule_regex_negative) {

    std::regex reg{
        parser->getRegex(d19::WorkflowStringParser::ParsedTypes::rule)};

    // missing : and destination
    EXPECT_FALSE(std::regex_match("x<1234", reg));

    // missing destination
    EXPECT_FALSE(std::regex_match("a<1234:", reg));

    // upper case workflowkey
    EXPECT_FALSE(std::regex_match("x>1234:TR", reg));

    EXPECT_FALSE(std::regex_match("TR", reg));
}

TEST_F(FixtureWorkflowStringParser, Workflow_regex_positive) {

    std::regex reg{
        parser->getRegex(d19::WorkflowStringParser::ParsedTypes::workflow)};

    EXPECT_TRUE(std::regex_match("qzd{m>2734:R,x>697:A,kmp}", reg));
    EXPECT_TRUE(std::regex_match("hi{x<34:R,a>697:A,R}", reg));
    EXPECT_TRUE(std::regex_match("hgf{s<54:R,a>697:A,R}", reg));
    EXPECT_TRUE(std::regex_match("qzd{x<87:R,s<697:A,x>7:R,gh}", reg));

    EXPECT_TRUE(std::regex_match("qzd{A}", reg));
    EXPECT_TRUE(std::regex_match("fa{R}", reg));
    EXPECT_TRUE(std::regex_match("bh{tr}", reg));

    EXPECT_TRUE(std::regex_match("bh{x>0:tr,A}", reg));
    EXPECT_TRUE(std::regex_match("tr{m>10:A,gr}", reg));
    EXPECT_TRUE(std::regex_match("grb{a<300:bh,f}", reg));
    EXPECT_TRUE(std::regex_match("kmp{s>505:hgf,re}", reg));
}

TEST_F(FixtureWorkflowStringParser, Workflow_regex_negative) {

    std::regex reg{
        parser->getRegex(d19::WorkflowStringParser::ParsedTypes::workflow)};

    // missing last rule destination
    EXPECT_FALSE(std::regex_match("qzd{x<2734:R,a>697:A}", reg));

    // missing braces
    EXPECT_FALSE(std::regex_match("qzdx<2734:R,a>697:A,R", reg));

    // upper case workflowkey
    EXPECT_FALSE(std::regex_match("x>1234:TR", reg));

    EXPECT_FALSE(std::regex_match("TR", reg));
}

TEST_F(FixtureWorkflowStringParser, Machinepart) {

    auto parsed = parser->fromStringMachinePart("{x=342,m=311,a=770,s=1594}");

    EXPECT_EQ(parsed.get(d19::MachinePartKey::X), 342);
    EXPECT_EQ(parsed.get(d19::MachinePartKey::M), 311);
    EXPECT_EQ(parsed.get(d19::MachinePartKey::A), 770);
    EXPECT_EQ(parsed.get(d19::MachinePartKey::S), 1594);
}

TEST_F(FixtureWorkflowStringParser, fromStringXXX_regex_exception) {

    // MachinePart - missing a=XXX
    EXPECT_THROW(parser->fromStringMachinePart("{m=311,a=770,s=1594}"),
                 std::invalid_argument);

    // RuleComparator - false operation
    EXPECT_THROW(parser->fromStringRuleComparator("m=2734"),
                 std::invalid_argument);

    // Rule - missing destination
    EXPECT_THROW(parser->fromStringRule(0, "m<2734:"), std::invalid_argument);

    // Workflow - missing last destination
    EXPECT_THROW(parser->fromStringWorkflow("qzd{m>2734:R,x>697:A}"),
                 std::invalid_argument);
}

TEST_F(FixtureWorkflowStringParser, Workflow) {

    auto parsed = parser->fromStringWorkflow("qzd{m>2734:R,x>697:A,kmp}");

    EXPECT_EQ(parsed.getKey(), "qzd");
    auto rule = parsed.getRules().first;
    auto comp = rule->getComp().value();
    EXPECT_EQ(d19::RuleComparator(d19::MachinePartKey::M,
                                  d19::RuleOperation::greater, 2734),
              comp);

    EXPECT_EQ(
        std::holds_alternative<d19::MachinePartStatus>(rule->getDestination()),
        true);
    EXPECT_EQ(std::get<d19::MachinePartStatus>(rule->getDestination()),
              d19::MachinePartStatus::rejected);

    ++rule;
    EXPECT_EQ(rule->getComp().value(),
              d19::RuleComparator(d19::MachinePartKey::X,
                                  d19::RuleOperation::greater, 697));

    EXPECT_EQ(
        std::holds_alternative<d19::MachinePartStatus>(rule->getDestination()),
        true);
    EXPECT_EQ(std::get<d19::MachinePartStatus>(rule->getDestination()),
              d19::MachinePartStatus::accepted);

    ++rule;
    EXPECT_EQ(std::holds_alternative<std::string>(rule->getDestination()),
              true);
    EXPECT_EQ(std::get<std::string>(rule->getDestination()), "kmp");

    parsed = parser->fromStringWorkflow("bmn{a>2630:gr,a>2504:R,A}");

    EXPECT_EQ(parsed.getKey(), "bmn");

    rule = parsed.getRules().first;
    comp = rule->getComp().value();
    EXPECT_EQ(comp, d19::RuleComparator(d19::MachinePartKey::A,
                                        d19::RuleOperation::greater, 2630));

    EXPECT_EQ(std::holds_alternative<std::string>(rule->getDestination()),
              true);
    EXPECT_EQ(std::get<std::string>(rule->getDestination()), "gr");

    ++rule;
    EXPECT_EQ(rule->getComp().value(),
              d19::RuleComparator(d19::MachinePartKey::A,
                                  d19::RuleOperation::greater, 2504));

    EXPECT_EQ(
        std::holds_alternative<d19::MachinePartStatus>(rule->getDestination()),
        true);
    EXPECT_EQ(std::get<d19::MachinePartStatus>(rule->getDestination()),
              d19::MachinePartStatus::rejected);

    ++rule;
    EXPECT_EQ(
        std::holds_alternative<d19::MachinePartStatus>(rule->getDestination()),
        true);
    EXPECT_EQ(std::get<d19::MachinePartStatus>(rule->getDestination()),
              d19::MachinePartStatus::accepted);
}