#include <Workflow.hpp>
#include <gtest/gtest.h>

#include <WorkflowStringParser.hpp>
#include <logging/DummyLogger.hpp>
#include <logging/FileLogger.hpp>

#include <limits.h>

using namespace d19;

class FixtureWorkflow : public ::testing::Test {
  protected:
    void SetUp(void) {
        logger = std::make_shared<d19::DummyLogger>();
        parser = std::make_unique<d19::WorkflowStringParser>(logger);
    }

    void TearDown(void) {}

    std::shared_ptr<d19::ILog> logger;
    std::unique_ptr<d19::WorkflowStringParser> parser;
};

// --------------------- MachinePart ------------------------

TEST_F(FixtureWorkflow, MachinePart_sum) {

    EXPECT_EQ(parser->fromStringMachinePart("{x=0,m=0,a=0,s=0}").sum(), 0);

    EXPECT_EQ(parser->fromStringMachinePart("{x=100,m=0,a=0,s=0}").sum(), 100);

    EXPECT_EQ(parser->fromStringMachinePart("{x=0,m=500,a=0,s=0}").sum(), 500);

    EXPECT_EQ(parser->fromStringMachinePart("{x=100,m=0,a=200,s=0}").sum(),
              300);

    EXPECT_EQ(parser->fromStringMachinePart("{x=0,m=0,a=0,s=300}").sum(), 300);

    EXPECT_EQ(parser->fromStringMachinePart("{x=1,m=2,a=3,s=4}").sum(), 10);
}

TEST_F(FixtureWorkflow, MachinePart_get) {

    d19::MachinePart part{parser->fromStringMachinePart("{x=1,m=2,a=3,s=4}")};

    EXPECT_EQ(part.get(d19::MachinePartKey::X), 1);
    EXPECT_EQ(part.get(d19::MachinePartKey::M), 2);
    EXPECT_EQ(part.get(d19::MachinePartKey::A), 3);
    EXPECT_EQ(part.get(d19::MachinePartKey::S), 4);
}

TEST_F(FixtureWorkflow, MachinePart_to_string) {

    d19::MachinePart part{parser->fromStringMachinePart("{x=1,m=2,a=3,s=4}")};

    EXPECT_EQ(part.to_string(), "{x=1,m=2,a=3,s=4}");
}

// -------------------- Workflow -------------------------------

TEST_F(FixtureWorkflow, Workflow_copyOfInput) {

    std::vector<d19::Rule> inputRules = {parser->fromStringRule(0, "x<230:ae"),
                                         parser->fromStringRule(1, "s>20:qs")};

    d19::Workflow wf{"test", inputRules};

    // Change local vector
    inputRules.pop_back();
    inputRules.push_back(parser->fromStringRule(1, "s>20:zrt"));

    auto [begin, end] = wf.getRules();

    // Local vector change should not have changes the wf
    EXPECT_EQ(std::get<std::string>(begin->getDestination()), "ae");

    // Check Local vector change
    EXPECT_EQ(std::get<std::string>(inputRules.at(1).getDestination()), "zrt");
}

TEST_F(FixtureWorkflow, Workflow_sortMachinePart_positive) {

    std::vector<d19::MachinePart> parts = {
        // x, m, a, s
        d19::MachinePart{787, 2655, 1222, 2876},
        d19::MachinePart{1679, 44, 2067, 496},
        d19::MachinePart{2036, 264, 79, 2244},
        d19::MachinePart{2461, 1339, 466, 291},
        d19::MachinePart{2127, 1623, 2188, 1013},
        d19::MachinePart{1679, 2099, 2067, 496}};

    Workflow wf = parser->fromStringWorkflow("px{a<2006:qkq,m>2090:A,rfg}");

    EXPECT_EQ(std::get<std::string>(wf.sortMachinePart(parts[0])), "qkq");
    EXPECT_EQ(std::get<std::string>(wf.sortMachinePart(parts[1])), "rfg");
    EXPECT_EQ(std::get<std::string>(wf.sortMachinePart(parts[2])), "qkq");
    EXPECT_EQ(std::get<std::string>(wf.sortMachinePart(parts[3])), "qkq");
    EXPECT_EQ(std::get<std::string>(wf.sortMachinePart(parts[4])), "rfg");
    EXPECT_EQ(std::get<d19::MachinePartStatus>(wf.sortMachinePart(parts[5])),
              d19::MachinePartStatus::accepted);
}

TEST_F(FixtureWorkflow, Workflow_to_string) {
    EXPECT_EQ(parser->fromStringWorkflow("xyz{A}").to_string(), "xyz{A}");
    EXPECT_EQ(parser->fromStringWorkflow("xyz{a>50:R,gr}").to_string(),
              "xyz{a>50:R,gr}");
    EXPECT_EQ(parser->fromStringWorkflow("tzz{a>50:r,s<8000:gr,R}").to_string(),
              "tzz{a>50:r,s<8000:gr,R}");
}

TEST_F(FixtureWorkflow, Workflow_getKey) {
    EXPECT_EQ(parser->fromStringWorkflow("xyz{A}").getKey(), "xyz");
    EXPECT_EQ(parser->fromStringWorkflow("hyp{a>50:r,gr}").getKey(), "hyp");
    EXPECT_EQ(parser->fromStringWorkflow("tzz{a>50:r,s<110:gr,R}").getKey(),
              "tzz");
}

TEST_F(FixtureWorkflow, Workflow_getRules) {
    d19::Workflow wf = parser->fromStringWorkflow("tzz{a>50:r,s<8000:gr,R}");
    auto [begin, end] = wf.getRules();

    EXPECT_EQ((*begin).to_string(), "a>50:r");
    EXPECT_EQ((*(++begin)).to_string(), "s<8000:gr");
    EXPECT_EQ((*(++begin)).to_string(), "R");
}

// ---------------- Rule -----------------------

TEST_F(FixtureWorkflow, RuleNE) {

    d19::Rule r1 = {0,
                    d19::RuleComparator{d19::MachinePartKey::A,
                                        d19::RuleOperation::less, 2006},
                    "qkq"};
    d19::Rule r2 = {1,

                    d19::RuleComparator{d19::MachinePartKey::M,
                                        d19::RuleOperation::greater, 2090},
                    d19::MachinePartStatus::accepted};

    EXPECT_NE(r1, r2);

    d19::Rule r3 = {1,

                    d19::RuleComparator{d19::MachinePartKey::M,
                                        d19::RuleOperation::greater, 2091},
                    d19::MachinePartStatus::accepted};

    EXPECT_NE(r2, r3);
}

TEST_F(FixtureWorkflow, RuleEQ) {

    d19::Rule r1{0,
                 d19::RuleComparator{d19::MachinePartKey::A,
                                     d19::RuleOperation::less, 2006},
                 "qkq"};
    d19::Rule r2{0,

                 d19::RuleComparator{d19::MachinePartKey::A,
                                     d19::RuleOperation::less, 2006},
                 "qkq"};

    EXPECT_EQ(r1, r2);
}

TEST_F(FixtureWorkflow, Rule_getOrderID) {
    EXPECT_EQ(parser->fromStringRule(0, "x>60:A").getOrderId(), 0);
    EXPECT_EQ(parser->fromStringRule(100, "x>60:A").getOrderId(), 100);
    EXPECT_EQ(parser->fromStringRule(UINT_MAX, "x>60:A").getOrderId(),
              UINT_MAX);
}

TEST_F(FixtureWorkflow, Rule_getComp) {
    EXPECT_EQ(parser->fromStringRule(0, "x>60:A").getComp().value(),
              parser->fromStringRuleComparator("x>60"));

    EXPECT_EQ(parser->fromStringRule(100, "A").getComp(), std::nullopt);
    EXPECT_EQ(parser->fromStringRule(100, "R").getComp().has_value(), false);
}

TEST_F(FixtureWorkflow, Rule_getDestination) {
    EXPECT_EQ(std::get<d19::MachinePartStatus>(
                  parser->fromStringRule(0, "x>60:A").getDestination()),
              d19::MachinePartStatus::accepted);

    EXPECT_EQ(std::get<d19::MachinePartStatus>(
                  parser->fromStringRule(0, "R").getDestination()),
              d19::MachinePartStatus::rejected);

    EXPECT_EQ(std::get<std::string>(
                  parser->fromStringRule(0, "s<500:gr").getDestination()),
              "gr");
}

TEST_F(FixtureWorkflow, Rule_to_string) {
    EXPECT_EQ(parser->fromStringRule(0, "x>60:A").to_string(), "x>60:A");

    EXPECT_EQ(parser->fromStringRule(0, "R").to_string(), "R");

    EXPECT_EQ(parser->fromStringRule(0, "s<500:gr").to_string(), "s<500:gr");
}

// ------------------- Rule Comparator -----------------------

TEST_F(FixtureWorkflow, RuleComparatorEQ) {

    d19::RuleComparator r1{d19::MachinePartKey::A, d19::RuleOperation::less,
                           2006};
    d19::RuleComparator r2{d19::MachinePartKey::A, d19::RuleOperation::less,
                           2006};
    EXPECT_EQ(r1, r2);
}

TEST_F(FixtureWorkflow, RuleComparatorNE) {

    d19::RuleComparator r1{d19::MachinePartKey::A, d19::RuleOperation::less,
                           2001};
    d19::RuleComparator r2{d19::MachinePartKey::A, d19::RuleOperation::less,
                           2006};
    EXPECT_NE(r1, r2);
}

TEST_F(FixtureWorkflow, RuleComparator_match) {

    // x
    d19::RuleComparator comp{parser->fromStringRuleComparator("x>40")};

    EXPECT_FALSE(
        comp.match(parser->fromStringMachinePart("{x=1,m=2,a=3,s=4}")));
    EXPECT_TRUE(
        comp.match(parser->fromStringMachinePart("{x=400,m=2,a=3,s=4}")));

    // m
    comp = {parser->fromStringRuleComparator("m<555")};

    EXPECT_FALSE(
        comp.match(parser->fromStringMachinePart("{x=0,m=2000,a=74,s=5}")));
    EXPECT_TRUE(
        comp.match(parser->fromStringMachinePart("{x=400,m=2,a=3,s=4}")));

    // a
    comp = {parser->fromStringRuleComparator("a>7")};

    EXPECT_FALSE(
        comp.match(parser->fromStringMachinePart("{x=0,m=2000,a=0,s=5}")));
    EXPECT_TRUE(
        comp.match(parser->fromStringMachinePart("{x=400,m=2,a=8,s=4}")));

    // s
    comp = {parser->fromStringRuleComparator("s<400")};

    EXPECT_FALSE(
        comp.match(parser->fromStringMachinePart("{x=0,m=2000,a=0,s=400}")));
    EXPECT_TRUE(
        comp.match(parser->fromStringMachinePart("{x=400,m=2,a=8,s=399}")));
}

TEST_F(FixtureWorkflow, RuleComparator_to_string) {

    EXPECT_EQ(parser->fromStringRuleComparator("x>40").to_string(), "x>40");
    EXPECT_EQ(parser->fromStringRuleComparator("m<400").to_string(), "m<400");
    EXPECT_EQ(parser->fromStringRuleComparator("a>0").to_string(), "a>0");
    EXPECT_EQ(parser->fromStringRuleComparator("s<10000000").to_string(),
              "s<10000000");
}