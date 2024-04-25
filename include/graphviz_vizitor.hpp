#pragma once

#include "program.hpp"

std::string visit(const NStatementPtr ptr, size_t &id);

SIMPLE_VISIT(Number, node.value());

SIMPLE_VISIT(Name, node.value());

SIMPLE_VISIT(Power, "**");
SIMPLE_VISIT(Abs, "abs");
SIMPLE_VISIT(Not, "not");

SIMPLE_VISIT(Mul, "*");
SIMPLE_VISIT(Div, "/");
SIMPLE_VISIT(Mod, "mod");
SIMPLE_VISIT(Rem, "rem");

SIMPLE_VISIT(UnPlus, "+");
SIMPLE_VISIT(UnMinus, "-");

SIMPLE_VISIT(BiPlus, "+");
SIMPLE_VISIT(BiMinus, "-");
SIMPLE_VISIT(BiAnd, "&");

SIMPLE_VISIT(Low, "<");
SIMPLE_VISIT(LowEq, "<=");
SIMPLE_VISIT(Eq, "=");
SIMPLE_VISIT(NotEq, "/>");
SIMPLE_VISIT(Great, ">");
SIMPLE_VISIT(GreatEq, ">=");

SIMPLE_VISIT(And, "and");
SIMPLE_VISIT(Or, "or");
SIMPLE_VISIT(Xor, "xor");

std::string visit(const NLogicalOperationPtr ptr, size_t &id)
{
    assert(ptr != nullptr);

    const auto this_id = id;

    return std::visit(
        [&id, this_id](const auto node) -> std::string
        {
            const auto child_id = ++id;
            return std::format(
                "{} [label = \"{}\"] {{ {} }} {} -- {} ", 
                this_id, "logical operation",
                visit(node, id),
                this_id, child_id
            );
        },
        *ptr
    );
}

std::string visit(const NRelationOperationPtr ptr, size_t &id)
{
    assert(ptr != nullptr);

    const auto this_id = id;

    return std::visit(
        [&id, this_id](const auto node) -> std::string
        {
            const auto child_id = ++id;
            return std::format(
                "{} [label = \"{}\"] {{ {} }} {} -- {} ", 
                this_id, "relation operation",
                visit(node, id),
                this_id, child_id
            );
        },
        *ptr
    );
}

std::string visit(const NUnOperationPtr ptr, size_t &id)
{
    assert(ptr != nullptr);

    const auto this_id = id;

    return std::visit(
        [&id, this_id](const auto node) -> std::string
        {
            const auto child_id = ++id;
            return std::format(
                "{} [label = \"{}\"] {{ {} }} {} -- {} ", 
                this_id, "unary operation",
                visit(node, id),
                this_id, child_id
            );
        },
        *ptr
    );
}

std::string visit(const NBiOperationPtr ptr, size_t &id)
{
    assert(ptr != nullptr);

    const auto this_id = id;

    return std::visit(
        [&id, this_id](const auto node) -> std::string
        {
            const auto child_id = ++id;
            return std::format(
                "{} [label = \"{}\"] {{ {} }} {} -- {} ", 
                this_id, "binary operation",
                visit(node, id),
                this_id, child_id
            );
        },
        *ptr
    );
}

std::string visit(const NMdmrOperationPtr ptr, size_t &id)
{
    assert(ptr != nullptr);

    const auto this_id = id;

    return std::visit(
        [&id, this_id](const auto node) -> std::string
        {
            const auto child_id = ++id;
            return std::format(
                "{} [label = \"{}\"] {{ {} }} {} -- {} ", 
                this_id, "mdmr operation",
                visit(node, id),
                this_id, child_id
            );
        },
        *ptr
    );
}

std::string visit(const NPrimaryPtr ptr, size_t &id)
{
    assert(ptr != nullptr);

    const auto this_id = id;

    return std::visit(
        [&id, this_id](const auto node) -> std::string
        {
            using Node = std::remove_const_t<decltype(node)>;

            if constexpr (std::is_same_v<Node, NStatementPtr>)
            {
                const auto l_bracket_id = ++id;
                const auto l_bracket = std::format("{} [label = \"(\"]", l_bracket_id);

                const auto child_id = ++id;
                const auto child = visit(node, id);

                const auto r_bracket_id = ++id;
                const auto r_bracket = std::format("{} [label = \")\"]", r_bracket_id);
                return std::format(
                    "{} [label = \"{}\"] {{ {} }} {{ {} }} {{ {} }} {} -- {} {} -- {} {} -- {} ", 
                    this_id, "primary",
                    l_bracket,
                    child,
                    r_bracket,
                    this_id, l_bracket_id,
                    this_id, child_id,
                    this_id, r_bracket_id
                );
            }
            else
            {
                const auto child_id = ++id;
                return std::format(
                    "{} [label = \"{}\"] {{ {} }} {} -- {} ", 
                    this_id, "primary",
                    visit(node, id),
                    this_id, child_id
                );
            }
        },
        *ptr
    );
}

std::string visit(const NFactorPtr ptr, size_t &id)
{
    assert(ptr != nullptr);

    const auto this_id = id;

    return std::visit(
        [&id, this_id](const auto node) -> std::string
        {
            using Node = std::remove_const_t<decltype(node)>;

            std::string result;
            if constexpr (std::is_same_v<Node, NFactorV1>)
            {
                auto child_id = ++id;
                result = std::format(
                    "{} [label = \"{}\"] {{ {} }} {} -- {} ",
                    this_id, "factor v1",
                    visit(node.primary(), id),
                    this_id, child_id
                );

                for (const auto &chunk : node.tail())
                {
                    child_id = ++id;
                    std::format_to(
                        std::back_inserter(result),
                        "{{ {} }} {} -- {} ",
                        visit(chunk.power(), id),
                        this_id, child_id
                    );

                    child_id = ++id;
                    std::format_to(
                        std::back_inserter(result),
                        "{{ {} }} {} -- {} ",
                        visit(chunk.primary(), id),
                        this_id, child_id
                    );
                }
            }
            else if constexpr (std::is_same_v<Node, NFactorV2>)
            {
                result = std::format(
                    "{} [label = \"{}\"] ",
                    this_id, "factor v2"
                );

                const auto first_child_id = ++id;
                std::format_to(
                    std::back_inserter(result),
                    "{{ {} }} {} -- {} ",
                    visit(node.abs(), id),
                    this_id, first_child_id
                );

                const auto second_child_id = ++id;
                std::format_to(
                    std::back_inserter(result),
                    "{{ {} }} {} -- {} ",
                    visit(node.primary(), id),
                    this_id, second_child_id
                );
            }
            else if constexpr (std::is_same_v<Node, NFactorV3>)
            {
                result = std::format(
                    "{} [label = \"{}\"] ",
                    this_id, "factor v3"
                );

                const auto first_child_id = ++id;
                std::format_to(
                    std::back_inserter(result),
                    "{{ {} }} {} -- {} ",
                    visit(node.nix(), id),
                    this_id, first_child_id
                );

                const auto second_child_id = ++id;
                std::format_to(
                    std::back_inserter(result),
                    "{{ {} }} {} -- {} ",
                    visit(node.primary(), id),
                    this_id, second_child_id
                );
            }
            else
            {
                static_assert(std::is_same_v<Node, NFactorV1>);
            }
            return result;
        },
        *ptr
    );
}

std::string visit(const NTermPtr ptr, size_t &id)
{
    assert(ptr != nullptr);

    const auto this_id = id;

    return std::visit(
        [&id, this_id](const NTermV1 node) -> std::string
        {
            auto child_id = ++id;
            std::string result = std::format(
                "{} [label = \"{}\"] {{ {} }} {} -- {} ",
                this_id, "term",
                visit(node.factor(), id),
                this_id, child_id
            );

            for (const auto &chunk : node.tail())
            {
                child_id = ++id;
                std::format_to(
                    std::back_inserter(result),
                    "{{ {} }} {} -- {} ",
                    visit(chunk.mdmrOperation(), id),
                    this_id, child_id
                );

                child_id = ++id;
                std::format_to(
                    std::back_inserter(result),
                    "{{ {} }} {} -- {} ",
                    visit(chunk.factor(), id),
                    this_id, child_id
                );
            }

            return result;
        },
        *ptr
    );
}

std::string visit(const NSimplestatementPtr ptr, size_t &id)
{
    assert(ptr != nullptr);

    const auto this_id = id;

    return std::visit(
        [&id, this_id](const NSimplestatementV1 node) -> std::string
        {
            std::string result = std::format(
                "{} [label = \"{}\"] ",
                this_id, "simple statement"
            );

            if (node.unOperation() != nullptr)
            {
                auto child_id = ++id;
                std::format_to(
                    std::back_inserter(result),
                    "{{ {} }} {} -- {} ",
                    visit(node.unOperation(), id),
                    this_id, child_id
                );
            }

            auto child_id = ++id;
            std::format_to(
                std::back_inserter(result),
                "{{ {} }} {} -- {} ",
                visit(node.term(), id),
                this_id, child_id
            );

            for (const auto &chunk : node.tail())
            {
                child_id = ++id;
                std::format_to(
                    std::back_inserter(result),
                    "{{ {} }} {} -- {} ",
                    visit(chunk.biOperation(), id),
                    this_id, child_id
                );

                child_id = ++id;
                std::format_to(
                    std::back_inserter(result),
                    "{{ {} }} {} -- {} ",
                    visit(chunk.term(), id),
                    this_id, child_id
                );
            }

            return result;
        },
        *ptr
    );
}

std::string visit(const NRelationPtr ptr, size_t &id)
{
    assert(ptr != nullptr);

    const auto this_id = id;

    return std::visit(
        [&id, this_id](const NRelationV1 node) -> std::string
        {
            auto child_id = ++id;
            std::string result = std::format(
                "{} [label = \"{}\"] {{ {} }} {} -- {} ",
                this_id, "relation",
                visit(node.simpleStatement(), id),
                this_id, child_id
            );

            for (const auto &chunk : node.tail())
            {
                child_id = ++id;
                std::format_to(
                    std::back_inserter(result),
                    "{{ {} }} {} -- {} ",
                    visit(chunk.relationOperation(), id),
                    this_id, child_id
                );

                child_id = ++id;
                std::format_to(
                    std::back_inserter(result),
                    "{{ {} }} {} -- {} ",
                    visit(chunk.simpleStatement(), id),
                    this_id, child_id
                );
            }

            return result;
        },
        *ptr
    );
}

std::string visit(const NStatementPtr ptr, size_t &id)
{
    assert(ptr != nullptr);

    const auto this_id = id;

    return std::visit(
        [&id, this_id](const NStatementV1 node) -> std::string
        {
            auto child_id = ++id;
            std::string result = std::format(
                "{} [label = \"{}\"] {{ {} }} {} -- {} ",
                this_id, "statement",
                visit(node.relation(), id),
                this_id, child_id
            );

            for (const auto &chunk : node.tail())
            {
                child_id = ++id;
                std::format_to(
                    std::back_inserter(result),
                    "{{ {} }} {} -- {} ",
                    visit(chunk.logicalOperation(), id),
                    this_id, child_id
                );

                child_id = ++id;
                std::format_to(
                    std::back_inserter(result),
                    "{{ {} }} {} -- {} ",
                    visit(chunk.relation(), id),
                    this_id, child_id
                );
            }

            return result;
        },
        *ptr
    );
}

std::string visit(const NAssignmentPtr ptr, size_t &id)
{
    assert(ptr != nullptr);

    const auto this_id = id;

    return std::visit(
        [&id, this_id] (const NAssignmentV1 node) -> std::string
        {
            std::string result = std::format(
                "{} [label = \"{}\"] ", 
                this_id, "assignment"
            );

            const auto name_child_id = ++id;
            std::format_to(
                std::back_inserter(result),
                "{{ {} }} {} -- {} ",
                visit(node.name(), id),
                this_id, name_child_id
            );

            const auto assignment_id = ++id;
            std::format_to(
                std::back_inserter(result),
                "{{ {} [label = \":=\"] }} {} -- {} ",
                assignment_id,
                this_id, assignment_id
            );

            const auto statement_child_id = ++id;
            std::format_to(
                std::back_inserter(result),
                "{{ {} }} {} -- {} ",
                visit(node.statement(), id),
                this_id, statement_child_id
            );

            return result;
        },
        *ptr
    );
}

std::string visit(const NTailPtr ptr, size_t &id)
{
    assert(ptr != nullptr);

    const auto this_id = id;

    return std::visit(
        [&id, this_id](const auto node) -> std::string
        {
            using Node = std::remove_const_t<decltype(node)>;

            std::string result;
            if constexpr (std::is_same_v<Node, NTailV1>)
            {
                result = std::format(
                    "{} [label = \"{}\"] ",
                    this_id, "tail v1"
                );

                const auto d_c_id = ++id;
                std::format_to(
                    std::back_inserter(result),
                    "{{ {} [label = \";\"] }} {} -- {} ",
                    d_c_id,
                    this_id, d_c_id
                );

                const auto assignment_child_id = ++id;
                std::format_to(
                    std::back_inserter(result),
                    "{{ {} }} {} -- {} ",
                    visit(node.assignment(), id),
                    this_id, assignment_child_id
                );

                const auto tail_child_id = ++id;
                std::format_to(
                    std::back_inserter(result),
                    "{{ {} }} {} -- {} ",
                    visit(node.tail(), id),
                    this_id, tail_child_id
                );
            }
            else if constexpr (std::is_same_v<Node, NTailV2>)
            {
                return std::format("{} [label = \"{}\"]", this_id, "tail v2");
            }
            else
            {
                static_assert(std::is_same_v<Node, NTailV1>);
            }
            return result;
        },
        *ptr
    );
}

std::string visit(const NListAssignmentPtr ptr, size_t &id)
{
    assert(ptr != nullptr);

    const auto this_id = id;

    return std::visit(
        [&id, this_id](const NListAssignmentV1 node) -> std::string
        {
            auto result = std::format(
                "{} [label = \"{}\"] ",
                this_id, "list assignment"
            );

            const auto assignment_child_id = ++id;
            std::format_to(
                std::back_inserter(result),
                "{{ {} }} {} -- {} ",
                visit(node.assignment(), id),
                this_id, assignment_child_id
            );

            const auto tail_child_id = ++id;
            std::format_to(
                std::back_inserter(result),
                "{{ {} }} {} -- {} ",
                visit(node.tail(), id),
                this_id, tail_child_id
            );

            return result;
        },
        *ptr
    );
};

std::string visit(const NBlockPtr ptr, size_t &id)
{
    assert(ptr != nullptr);

    const auto this_id = id;

    return std::visit(
        [&id, this_id](const NBlockV1 node) -> std::string
        {
            auto child_id = ++id;
            std::string result = std::format(
                "{} [label = \"{}\"] {{ {} }} {} -- {} ",
                this_id, "block",
                visit(node.listAssignment(), id),
                this_id, child_id
            );

            for (const auto &chunk : node.tail())
            {
                child_id = ++id;
                std::format_to(
                    std::back_inserter(result),
                    "{{ {} }} {} -- {} ",
                    visit(chunk.listAssignment(), id),
                    this_id, child_id
                );

                child_id = ++id;
                std::format_to(
                    std::back_inserter(result),
                    "{{ {} }} {} -- {} ",
                    visit(chunk.listAssignment(), id),
                    this_id, child_id
                );
            }

            return result;
        },
        *ptr
    );
}

std::string visit(const NProgramPtr ptr, size_t &id)
{
    assert(ptr != nullptr);

    const auto this_id = id;

    return std::visit(
        [&id, this_id](const NProgramV1 node) -> std::string
        {
            auto child_id = ++id;
            return std::format(
                "{} [label = \"{}\"] {{ {} }} {} -- {} ",
                this_id, "program",
                visit(node.block(), id),
                this_id, child_id
            );
        },
        *ptr
    );
}