
#include <TraceException.hpp>
#include <Workflow.hpp>
#include <WorkflowStringParser.hpp>
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <logging/DummyLogger.hpp>
#include <logging/ILog.hpp>
#include <memory>
#include <optional>
#include <regex>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>
#include <vector>

const char *REGEX_MACHINEPART{"\\{x=[0-9]+,m=[0-9]+,a=[0-9]+,s=[0-9]+\\}"};
const char *REGEX_RULECOMPARATOR{"[xmas][><][0-9]+"};
const char *REGEX_RULE{"([xmas][><][0-9]+:([RA]|[a-z]+))|([RA]|[a-z]+)"};
const char *REGEX_WORKFLOW{"[a-z]+\\{((([xmas][><][0-9]+:([RA]|[a-z]+))|(["
                           "RA]|[a-z]+)),)*([RA]|[a-z]+)\\}"};

d19::WorkflowStringParser::WorkflowStringParser(
    std::shared_ptr<d19::ILog> logger)
    : logger{logger} {};
d19::WorkflowStringParser::WorkflowStringParser()
    : logger{std::make_shared<d19::DummyLogger>()} {};

d19::MachinePart
d19::WorkflowStringParser::fromStringMachinePart(const std::string &str) const {
    if (str.empty()) {
        throw std::invalid_argument(throw_message("Passed string is empty!"));
    }

    std::regex reg{
        getRegex(d19::WorkflowStringParser::ParsedTypes::machinepart)};

    if (!std::regex_match(str, reg)) {
        throw std::invalid_argument(
            throw_message("Passed str does not match the format!"));
    }

    unsigned x{}, m{}, a{}, s{};

    // find '=' and ',' and convert via stoll. Get key and assign to object key
    size_t posStart{0};
    size_t posEnd{0};
    unsigned val{0};

    for (size_t i = 0; i < 4; ++i) {
        posStart = str.find('=', posStart + 1);
        posEnd = str.find(',', posEnd + 1);

        if (posEnd == std::string::npos) {
            posEnd = str.size() - 1;
        }
        auto strVal = str.substr(posStart + 1, (posEnd - 1) - posStart);
        val = std::stol(strVal);

        if (str.at(posStart - 1) == 'x') {
            x = val;
        }
        if (str.at(posStart - 1) == 'm') {
            m = val;
        }
        if (str.at(posStart - 1) == 'a') {
            a = val;
        }
        if (str.at(posStart - 1) == 's') {
            s = val;
        }
    }

    return d19::MachinePart{x, m, a, s};
};

std::variant<d19::MachinePartStatus, std::string>
fromStringDestination(const std::string &str) {
    std::variant<d19::MachinePartStatus, std::string> ret{};
    assert(str.size() >= 1);
    if (str == "R") {
        ret = d19::MachinePartStatus::rejected;
    } else if (str == "A") {
        ret = d19::MachinePartStatus::accepted;
    } else {
        ret = str;
    }
    return std::move(ret);
}

// qzd{m>2734:R,x>697:A,kmp}
// ----xxxxxxxx-------------
d19::Rule
d19::WorkflowStringParser::fromStringRule(unsigned orderID,
                                          const std::string &str) const {
    if (str.empty()) {
        throw std::invalid_argument(throw_message("Passed string is empty!"));
    }

    std::regex reg{getRegex(d19::WorkflowStringParser::ParsedTypes::rule)};

    if (!std::regex_match(str, reg)) {
        throw std::invalid_argument(
            throw_message("Passed str does not match the format!"));
    }

    std::optional<d19::RuleComparator> comp;
    std::variant<MachinePartStatus, std::string> destination{};

    if (str.contains(':')) {
        auto pos = str.find(':');
        d19::RuleComparator comp = fromStringRuleComparator(str.substr(0, pos));
        destination = fromStringDestination(str.substr(pos + 1));
        return {orderID, comp, destination};
    } else {
        destination = fromStringDestination(str);
        return {orderID, std::nullopt, destination};
    }
};

//qzd{m>2734:R,x>697:A,kmp}
//----xxxxxx---------------
d19::RuleComparator d19::WorkflowStringParser::fromStringRuleComparator(
    const std::string &str) const {
    if (str.empty()) {
        throw std::invalid_argument(throw_message("Passed string is empty!"));
    }

    std::regex reg{
        getRegex(d19::WorkflowStringParser::ParsedTypes::rulecomparator)};

    if (!std::regex_match(str, reg)) {
        throw std::invalid_argument(
            throw_message("Passed str does not match the format!"));
    }

    d19::MachinePartKey key{};
    d19::RuleOperation op{};

    switch (str.at(0)) {
    case 'x':
        key = d19::MachinePartKey::X;
        break;
    case 'm':
        key = d19::MachinePartKey::M;
        break;
    case 'a':
        key = d19::MachinePartKey::A;
        break;
    case 's':
        key = d19::MachinePartKey::S;
        break;
    default:
        throw std::invalid_argument(throw_message(
            "Switch case passed is not known, should be element of "
            "d19::MachinePartKey!"));
    }

    switch (str.at(1)) {
    case '>':
        op = d19::RuleOperation::greater;
        break;
    case '<':
        op = d19::RuleOperation::less;
        break;
    default:
        throw std::invalid_argument(throw_message(
            "Switch case passed is not known, should be element of "
            "d19::RuleOperation!"));
    }

    return {key, op, static_cast<unsigned>(std::stol(str.substr(2)))};
};

// qzd{m>2734:R,x>697:A,kmp}
d19::Workflow
d19::WorkflowStringParser::fromStringWorkflow(const std::string &str) const {
    if (str.empty()) {
        throw std::invalid_argument(throw_message("Passed string is empty!"));
    }

    std::regex reg{getRegex(d19::WorkflowStringParser::ParsedTypes::workflow)};

    if (!std::regex_match(str, reg)) {
        throw std::invalid_argument(
            throw_message("Passed str does not match the format!"));
    }

    // find workflow key
    const std::string workflowKey = str.substr(0, str.find('{'));

    // find rules
    std::vector<Rule> inputRules{};

    std::string rules_str =
        str.substr(str.find('{') + 1, (str.find('}')) - (str.find('{') + 1));
    size_t start{0};
    size_t end{};

    const size_t sepOccurence =
        std::count(rules_str.begin(), rules_str.end(), ',') + 1;
    for (size_t i = 0; i < sepOccurence; ++i) {
        if (i > 0) {
            start = rules_str.find(',', start + 1) + 1;
        }
        if (i == (sepOccurence - 1)) {
            end = rules_str.size();
        } else {
            end = rules_str.find(',', start + 1);
        }
        auto subString = rules_str.substr(start, (end - start));
        auto rule = fromStringRule(i, subString);
        inputRules.push_back(rule);
    }

    return std::move(d19::Workflow{workflowKey, inputRules});
};

std::string d19::WorkflowStringParser::getRegex(
    d19::WorkflowStringParser::ParsedTypes type) const {
    using ptype = d19::WorkflowStringParser::ParsedTypes;

    switch (type) {
    case ptype::machinepart:
        return REGEX_MACHINEPART;
    case ptype::rulecomparator:
        return REGEX_RULECOMPARATOR;
    case ptype::rule:
        return REGEX_RULE;
    case ptype::workflow:
        return REGEX_WORKFLOW;
    default:
        throw std::invalid_argument(throw_message(
            "Switch case passed is not known, should be element of "
            "d19::WorkflowStringParser::ParsedTypes!"));
    }
}
