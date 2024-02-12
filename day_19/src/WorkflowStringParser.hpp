#ifndef AOC_D19_WORKFLOWSTRINGPARSER_H
#define AOC_D19_WORKFLOWSTRINGPARSER_H

#include <Workflow.hpp>
#include <cstdint>
#include <logging/ILog.hpp>
#include <memory>
#include <optional>
#include <string>

namespace d19 {

class WorkflowStringParser {
  private:
    std::shared_ptr<d19::ILog> logger;

  public:
    explicit WorkflowStringParser(std::shared_ptr<d19::ILog> logger);
    WorkflowStringParser();

    /**
     * @brief Transform a string representation to a MachinePart.
     * Format of string: "{x=NUM,m=NUM,a=NUM,s=NUM}" with NUM being specifc numbers. 
     * 
     * @param str string to parse
     * @return MachinePart parsed part
     */
    [[nodiscard]] MachinePart
    fromStringMachinePart(const std::string &str) const;

    /**
     * @brief Transform a string representation to a Rule.
     * Example of a string of a Rule: "a>3333:R"
     * 
     * @param orderID Index of Rule in Workflow
     * @param str string to parse
     * @return Rule 
     */
    [[nodiscard]] Rule fromStringRule(unsigned orderID,
                                      const std::string &str) const;

    /**
     * @brief Transform a string representation to a RuleComparator.
     * Example of a string of a RuleComparator "s>2770"
     * 
     * @param str string to parse
     * @return RuleComparator 
     */
    [[nodiscard]] RuleComparator
    fromStringRuleComparator(const std::string &str) const;

    /**
     * @brief Transform a string representation to a Workflow.
     * Example of a string of a Workflow: "qqz{s>2770:qs,m<1801:hdj,R}"
     * 
     * @param str string to parse
     * @return Workflow 
     * 
     * @exception std::invalid_argument if str is empty
     */
    [[nodiscard]] Workflow fromStringWorkflow(const std::string &str) const;

    enum class ParsedTypes { machinepart, rulecomparator, rule, workflow };

    /**
     * @brief Get a regex string representation/format for 
     * the correct parsing of the other member functions
     * 
     * @param type 
     * @return std::string regex string format 
     */
    [[nodiscard]] std::string getRegex(ParsedTypes type) const;
};

} // namespace d19

#endif // AOC_D19_WORKFLOWSTRINGPARSER_H