#ifndef AOC_D19_WORKFLOW
#define AOC_D19_WORKFLOW

#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace d19 {

enum class MachinePartKey : uint8_t { X, M, A, S };

class MachinePart {
  private:
    unsigned x;
    unsigned m;
    unsigned a;
    unsigned s;

  public:
    MachinePart(unsigned x, unsigned m, unsigned a, unsigned s);

    /**
     * @brief Add all values of all MachinePart Keys
     * 
     * @return unsigned 
     */
    [[nodiscard]] unsigned sum() const;

    /**
     * @brief Return the value of a given element
     * 
     * @param key element to return a value for
     * @return unsigned 
     */
    [[nodiscard]] unsigned get(MachinePartKey key) const;

    /**
     * @brief Transform to string representation
     * 
     * @return std::string 
     */
    [[nodiscard]] std::string to_string() const;

    auto operator<=>(const MachinePart &) const = default;
};

enum class RuleOperation : uint8_t { less, greater };
enum class MachinePartStatus : uint8_t { accepted, rejected };

class RuleComparator {
    MachinePartKey key;
    RuleOperation operation;
    unsigned value;

  public:
    RuleComparator(MachinePartKey key, RuleOperation operation, unsigned value);
    auto operator<=>(const RuleComparator &) const = default;

    /**
     * @brief Compare a specifc MachinePart Element to the Comparator Key, 
     * dependent on the operation and value.  
     * 
     * @param part to compare an element of
     * @return true part matches
     * @return false part does no match
     */
    [[nodiscard]] bool match(MachinePart part) const;

    /**
     * @brief Transform to string representation
     * 
     * @return std::string 
     */
    [[nodiscard]] std::string to_string() const;
};

class Rule {
    unsigned orderId;
    std::optional<RuleComparator> comp;
    std::variant<MachinePartStatus, std::string> destination;

  public:
    /**
     * @brief Construct a new Rule object
     * 
     * @param orderId Order Identfier in Workflow
     * @param comp Element, Comparator and Value a MachinePart Element is compared to
     * @param destination Key of the next Workflow the part should be transferred to, if comp is true. 
     * Always true, if no comp exists.
     * 
     * @exception std::invalid_argument if held string in @a destination is empty
     */
    Rule(unsigned orderId, std::optional<RuleComparator> comp,
         std::variant<MachinePartStatus, std::string> destination);

    /**
     * @brief Get the Order Id
     * 
     * @return unsigned 
     */
    [[nodiscard]] unsigned getOrderId() const;

    /**
     * @brief Get the RuleComparator object, if existent
     * 
     * @return std::optional<RuleComparator> 
     */
    [[nodiscard]] std::optional<RuleComparator> getComp() const;

    /**
     * @brief Get the destination the rule points to
     * 
     * @return std::variant<MachinePartStatus, std::string> 
     */
    [[nodiscard]] std::variant<MachinePartStatus, std::string>
    getDestination() const;
    // TODO might be a type - with functions {isWorkflowKey(), isSorted()}

    /**
     * @brief Transform to string representation
     * 
     * @return std::string 
     */
    [[nodiscard]] std::string to_string() const;
    auto operator<=>(const Rule &) const = default;
};

class Workflow {
  private:
    std::string workflowKey;
    // TODO a special type might be created - strong_type for workflowKey
    std::vector<Rule> rules;

  public:
    /**
     * @brief Construct a new Workflow object. 
     * 
     * @param workflowKey Identifier other rules refer to with their destination
     * @param inputRules rules to sort parts with, order is important
     * 
     * @exception invalid_argument if @a workflowKey is empty
     * @exception invalid_argument if @a inputRules is empty
     */
    Workflow(const std::string &workflowKey,
             const std::vector<Rule> &inputRules);

    /**
     * @brief Get [begin, end] iterators for traversal of all rules 
     * 
     * @return std::pair<std::vector<Rule>::const_iterator,
     * std::vector<Rule>::const_iterator> 
     */
    std::pair<std::vector<Rule>::const_iterator,
              std::vector<Rule>::const_iterator>
    getRules();

    /**
     * @brief Traverse over all Rules and try a match with the part.
     * 
     * @param part To match against the rules
     * @return std::variant<MachinePartStatus, std::string> Destination to transfer the part to next
     */
    [[nodiscard]] std::variant<MachinePartStatus, std::string>
    sortMachinePart(MachinePart part) const;

    /**
     * @brief Get the Key of the Workflow
     * 
     * @return std::string 
     */
    [[nodiscard]] std::string getKey() const;

    /**
     * @brief Transform to string representation
     * 
     * @return std::string 
     */
    [[nodiscard]] std::string to_string() const;
};

} // namespace d19

#endif // AOC_D19_WORKFLOW