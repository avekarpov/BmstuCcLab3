#pragma once

#include "assignment.hpp"

struct NTail;
using NTailPtr = std::shared_ptr<NTail>;

class NTailV1
{
public:
    NTailV1(NAssignmentPtr assignment_ptr, NTailPtr tail_ptr)
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

class NTailV2
{};

struct NTail : std::variant<NTailV1, NTailV2>
{
    using std::variant<NTailV1, NTailV2>::variant;
};

inline NTailPtr parseTail(Lexer lexer)
{
    if (not lexer.isEnd())
    {
        if (lexer.getNext() != ';')
        {
            throw std::invalid_argument { "expected ; before assignment" };
        }

        skip(lexer, " \n");

        NTailV1 node { parseAssignment(lexer), parseTail(lexer) };
        return std::make_shared<NTail>(std::move(node));
    }
    else
    {
        NTailV2 node;
        return std::make_shared<NTail>(std::move(node));
    }

    return nullptr;
}
