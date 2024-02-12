#include <TraceException.hpp>
#include <Utility.hpp>
#include <Workflow.hpp>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>
#include <vector>

// ------------------------- MachinePart class -------------------------

d19::MachinePart::MachinePart(unsigned x, unsigned m, unsigned a, unsigned s)
    : x{x}, m{m}, a{a}, s{s} {};

unsigned d19::MachinePart::sum() const { return x + m + a + s; };

unsigned d19::MachinePart::get(MachinePartKey key) const {
    switch (key) {
    case d19::MachinePartKey::X:
        return x;
        break;
    case d19::MachinePartKey::M:
        return m;
        break;
    case d19::MachinePartKey::A:
        return a;
        break;
    case d19::MachinePartKey::S:
        return s;
        break;

    default:
        throw std::invalid_argument(
            throw_message("Argument passed is not known, should be element of "
                          "d19::MachinePartKey!"));
    }
}

std::string d19::MachinePart::to_string() const {
    return "{x=" + std::to_string(x) + ",m=" + std::to_string(m) +
           ",a=" + std::to_string(a) + ",s=" + std::to_string(s) + "}";
}

// ---------------------- RuleComparator class --------------------

d19::RuleComparator::RuleComparator(d19::MachinePartKey key,
                                    d19::RuleOperation operation,
                                    unsigned value)
    : key{key}, operation{operation}, value{value} {
    switch (key) {
    case d19::MachinePartKey::X:
        [[fallthrough]];
    case d19::MachinePartKey::M:
        [[fallthrough]];
    case d19::MachinePartKey::A:
        [[fallthrough]];
    case d19::MachinePartKey::S:
        break;
    default:
        throw std::invalid_argument(throw_message(
            "Switch case passed is not known, should be element of "
            "d19::RuleOperation!"));
    }

    switch (operation) {
    case d19::RuleOperation::less:
        [[fallthrough]];
    case d19::RuleOperation::greater:
        break;
    default:
        throw std::invalid_argument(throw_message(
            "Switch case passed is not known, should be element of "
            "d19::RuleOperation!"));
    }
};

bool d19::RuleComparator::match(const MachinePart part) const {
    switch (operation) {
    case d19::RuleOperation::less:
        return part.get(key) < value;
        break;
    case d19::RuleOperation::greater:
        return part.get(key) > value;
        break;
    default:
        throw std::invalid_argument(throw_message(
            "Switch case passed is not known, should be element of "
            "d19::RuleOperation!"));
    }
};

std::string d19::RuleComparator::to_string() const {
    std::string str{};
    switch (key) {
    case d19::MachinePartKey::X:
        str += "x";
        break;
    case d19::MachinePartKey::M:
        str += "m";
        break;
    case d19::MachinePartKey::A:
        str += "a";
        break;
    case d19::MachinePartKey::S:
        str += "s";
        break;
    default:
        throw std::invalid_argument(throw_message(
            "Switch case passed is not known, should be element of "
            "d19::MachinePartKey!"));
    }

    switch (operation) {
    case d19::RuleOperation::greater:
        str += ">";
        break;
    case d19::RuleOperation::less:
        str += "<";
        break;

    default:
        throw std::invalid_argument(throw_message(
            "Switch case passed is not known, should be element of "
            "d19::RuleOperation!"));
    }

    str += std::to_string(value);
    return str;
}

// --------------- Rule Class ---------------------

d19::Rule::Rule(unsigned orderId, std::optional<d19::RuleComparator> comp,
                std::variant<d19::MachinePartStatus, std::string> destination)
    : orderId{orderId}, comp{comp}, destination{destination} {

    if (std::holds_alternative<std::string>(destination) &&
        std::get<std::string>(destination).empty()) {
        throw std::invalid_argument(
            throw_message("String in parameter destination is empty()!"));
    }
};

unsigned d19::Rule::getOrderId() const { return orderId; };

std::optional<d19::RuleComparator> d19::Rule::getComp() const { return comp; };

std::variant<d19::MachinePartStatus, std::string>
d19::Rule::getDestination() const {
    return destination;
};

std::string d19::Rule::to_string() const {
    std::string str{};
    if (comp.has_value()) {
        str += comp.value().to_string() + ":";
    };
    if (std::holds_alternative<d19::MachinePartStatus>(destination)) {
        switch (std::get<d19::MachinePartStatus>(destination)) {
        case d19::MachinePartStatus::accepted:
            str += "A";
            break;
        case d19::MachinePartStatus::rejected:
            str += "R";
            break;

        default:
            throw std::invalid_argument(throw_message(
                "Switch case passed is not known, should be element of "
                "d19::MachinePartStatus!"));
        }

    } else {
        str += std::get<std::string>(destination);
    }
    return str;
}

// -------------------- Workflow Class ---------------------------

std::pair<std::vector<d19::Rule>::const_iterator,
          std::vector<d19::Rule>::const_iterator>
d19::Workflow::getRules() {
    return std::make_pair(rules.begin(), rules.end());
}

d19::Workflow::Workflow(const std::string &workflowKey,
                        const std::vector<Rule> &inputRules)
    : workflowKey{workflowKey}, rules{} {

    // Preserve object invariance
    if (inputRules.empty()) {
        throw std::invalid_argument(throw_message("Container is empty()!"));
    }

    if (workflowKey.empty()) {
        throw std::invalid_argument(throw_message("Key is empty()!"));
    }

    std::copy(inputRules.begin(), inputRules.end(), std::back_inserter(rules));
    assert(rules.size() == inputRules.size());
};

std::variant<d19::MachinePartStatus, std::string>
d19::Workflow::sortMachinePart(MachinePart part) const {

    for (auto it = rules.begin(); it != rules.end(); ++it) {
        if (it->getComp().has_value() && it->getComp().value().match(part)) {
            return it->getDestination();
        }
    }

    // Last Rule will be chosen, if no match ocurred
    return (--rules.end())->getDestination();
};

std::string d19::Workflow::getKey() const { return workflowKey; }

std::string d19::Workflow::to_string() const {
    std::string str{};
    str = workflowKey + vectorToString<d19::Rule>(rules, [](const auto &elem) {
              return elem.to_string();
          });
    return str;
}
