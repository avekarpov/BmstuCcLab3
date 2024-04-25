#pragma once

#include "tail.hpp"

struct NListAssignmentV1
{
public:
    NListAssignmentV1(NAssignmentPtr assignment_ptr, NTailPtr tail_ptr)
    : _assignment_ptr { validate(assignment_ptr) }
    , _tail_ptr { validate(tail_ptr) }
    {}

    NAssignmentPtr assignment() const
    {
        return _assignment_ptr;
    }

    NTailPtr tail() const
    {
        return _tail_ptr;
    }

private:
    static NAssignmentPtr validate(NAssignmentPtr ptr)
    {
        if (ptr == nullptr)
        {
            throw std::runtime_error { "expected not null assignment node" };
        }

        return ptr;
    }

    static NTailPtr validate(NTailPtr ptr)
    {
        if (ptr == nullptr)
        {
            throw std::runtime_error { "expected not null tail node" };
        }

        return ptr;
    }

private:
    NAssignmentPtr _assignment_ptr;
    NTailPtr _tail_ptr;
};

struct NListAssignment : std::variant<NListAssignmentV1>
{
    using std::variant<NListAssignmentV1>::variant;
};
using NListAssignmentPtr = std::shared_ptr<NListAssignment>;

inline NListAssignmentPtr parseListAssignment(Lexer lexer)
{
    NListAssignmentV1 node { parseAssignment(lexer), parseTail(lexer) };
    return std::make_shared<NListAssignment>(std::move(node));
}