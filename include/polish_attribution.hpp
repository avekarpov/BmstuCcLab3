#pragma once

#include "program.hpp"

using AttrValue = std::string;

#define DEC_ATTR(__NAME__) \
AttrValue attr(const N##__NAME__##Ptr ptr)

#define SIMPLE_ATTR(__NAME__, __CAST__) \
DEC_ATTR(__NAME__) \
{ \
    assert(ptr != nullptr); \
\
    return std::visit( \
        [](const auto node) -> AttrValue \
        { \
            return AttrValue { __CAST__ }; \
        }, \
        *ptr \
    ); \
} \
struct __attr##__NAME__##__ {}

SIMPLE_ATTR(Name, node.value());
SIMPLE_ATTR(Number, std::to_string(node.value()));

SIMPLE_ATTR(Power, "**");
SIMPLE_ATTR(Abs, "abs");
SIMPLE_ATTR(Not, "not");

SIMPLE_ATTR(Mul, "*");
SIMPLE_ATTR(Div, "/");
SIMPLE_ATTR(Mod, "mod");
SIMPLE_ATTR(Rem, "rem");

SIMPLE_ATTR(UnPlus, "u+");
SIMPLE_ATTR(UnMinus, "u-");

SIMPLE_ATTR(Low, "<");
SIMPLE_ATTR(LowEq, "<=");
SIMPLE_ATTR(Eq, "=");
SIMPLE_ATTR(NotEq, "/>");
SIMPLE_ATTR(Great, ">");
SIMPLE_ATTR(GreatEq, ">=");

SIMPLE_ATTR(And, "and");
SIMPLE_ATTR(Or, "or");
SIMPLE_ATTR(Xor, "xor");

SIMPLE_ATTR(BiPlus, "+");
SIMPLE_ATTR(BiMinus, "-");
SIMPLE_ATTR(BiAnd, "&");

#define TRANSIT_ATTR(__NAME__) \
DEC_ATTR(__NAME__) \
{ \
    assert(ptr != nullptr); \
\
    return std::visit( \
        [](const auto node_ptr) -> AttrValue \
        { \
            return attr(node_ptr); \
        }, \
        *ptr \
    ); \
} \
struct __attr##__NAME__##__ {}

DEC_ATTR(Statement);

TRANSIT_ATTR(Primary);
TRANSIT_ATTR(MdmrOperation);
TRANSIT_ATTR(BiOperation);
TRANSIT_ATTR(UnOperation);
TRANSIT_ATTR(RelationOperation);
TRANSIT_ATTR(LogicalOperation);

DEC_ATTR(Factor)
{
    assert(ptr != nullptr);

    return std::visit(
        [](const auto node) -> AttrValue
        {
            using Node = std::remove_const_t<decltype(node)>;

            AttrValue value;
            if constexpr (std::is_same_v<Node, NFactorV1>)
            {
                value = attr(node.primary());
                for (const auto &chunk : node.tail())
                {
                    value += " ";
                    value += attr(chunk.primary());
                    value += " ";
                    value += attr(chunk.power());
                }
            }
            else if constexpr (std::is_same_v<Node, NFactorV2>)
            {
                value = attr(node.primary()) + " " + attr(node.abs());
            }
            else if constexpr (std::is_same_v<Node, NFactorV3>)
            {
                value = attr(node.primary()) + " " + attr(node.nix());
            }
            else
            {
                static_assert(std::is_same_v<Node, NFactorV1>);
            }
            return value;
        },
        *ptr
    );
}

DEC_ATTR(Term)
{
    assert(ptr != nullptr);

    return std::visit(
        [](const NTermV1 node) -> AttrValue
        {
            AttrValue value = attr(node.factor());
            for (const auto &chunk : node.tail())
            {
                value += " ";
                value += attr(chunk.factor());
                value += " ";
                value += attr(chunk.mdmrOperation());
            }
            return value;
        },
        *ptr
    );
}

DEC_ATTR(Simplestatement)
{
    assert(ptr != nullptr);

    return std::visit(
        [](const NSimplestatementV1 node) -> AttrValue
        {
            AttrValue value = attr(node.term());
            if (node.unOperation() != nullptr)
            {
                value += " ";
                value += attr(node.unOperation());
            }
            for (const auto &chunk : node.tail())
            {
                value += " ";
                value += attr(chunk.term());
                value += " ";
                value += attr(chunk.biOperation());
            }
            return value;
        },
        *ptr
    );
}

DEC_ATTR(Relation)
{
    assert(ptr != nullptr);

    return std::visit(
        [](const NRelationV1 node) -> AttrValue
        {
            AttrValue value = attr(node.simpleStatement());
            for (const auto &chunk : node.tail())
            {
                value += " ";
                value += attr(chunk.simpleStatement());
                value += " ";
                value += attr(chunk.relationOperation());
            }
            return value;
        },
        *ptr
    );
}

DEC_ATTR(Statement)
{
    assert(ptr != nullptr);

    return std::visit(
        [](const NStatementV1 node) -> AttrValue
        {
            AttrValue value = attr(node.relation());
            for (const auto &chunk : node.tail())
            {
                value += " ";
                value += attr(chunk.relation());
                value += " ";
                value += attr(chunk.logicalOperation());
            }
            return value;
        },
        *ptr
    );
}

DEC_ATTR(Assignment);

DEC_ATTR(Tail)
{
    assert(ptr != nullptr);

    return std::visit(
        [](const auto node) -> AttrValue
        {
            using Node = std::remove_const_t<decltype(node)>;

            AttrValue value;
            if constexpr (std::is_same_v<Node, NTailV1>)
            {
                value = attr(node.assignment());
                if (node.tail() != nullptr)
                {
                    value += " ";
                    value += attr(node.tail()) + "; ";
                }
            }
            else if constexpr (std::is_same_v<Node, NTailV2>)
            {
                // empty
            }
            else
            {
                static_assert(std::is_same_v<Node, NTailV1>);
            }
            return value;
        },
        *ptr
    );
}

DEC_ATTR(ListAssignment)
{
    assert(ptr != nullptr);

    return std::visit(
        [](const NListAssignmentV1 node) -> AttrValue
        {
            return attr(node.assignment()) + " " + attr(node.tail());
        },
        *ptr
    );  
}

DEC_ATTR(Block)
{
    assert(ptr != nullptr);

    return std::visit(
        [](const NBlockV1 node) -> AttrValue
        {
            return attr(node.listAssignment());
        },
        *ptr
    );
}

DEC_ATTR(Program)
{
    assert(ptr != nullptr);

    return std::visit(
        [](const NProgramV1 node) -> AttrValue
        {
            return attr(node.block());
        },
        *ptr
    );
}

DEC_ATTR(Assignment)
{
    assert(ptr != nullptr);

    return std::visit(
        [](const auto node) -> AttrValue
        {
            using Node = std::remove_const_t<decltype(node)>;

            AttrValue value;
            if constexpr (std::is_same_v<Node, NAssignmentV1>)
            {
                value = attr(node.name());
                value += " ";
                value += attr(node.statement());
                value += " ";
                value += ":=";
            }
            else if constexpr (std::is_same_v<Node, NAssignmentV2>)
            {
                value = attr(node.block());
            }
            else
            {
                static_assert(std::is_same_v<Node, NAssignmentV1>);
            }
            return value;
        },
        *ptr
    );
}