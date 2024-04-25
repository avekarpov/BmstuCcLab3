#pragma once

#include "statement.hpp"

class NAssignmentV1
{
public:
    NAssignmentV1(NNamePtr name_ptr, NStatementPtr statement_ptr)
    : _name_ptr { name_ptr }
    , _statement_ptr { statement_ptr }
    {}
    
    NNamePtr name() const
    {
        return _name_ptr;
    }

    NStatementPtr statement() const
    {
        return _statement_ptr;
    }

private:
    static NNamePtr validate(NNamePtr ptr)
    {
        if (ptr == nullptr)
        {
            throw std::runtime_error { "expected not null name node" };
        }

        return ptr;
    }

    static NStatementPtr validate(NStatementPtr ptr)
    {
        if (ptr == nullptr)
        {
            throw std::runtime_error { "expected not null statement ptr" };
        }

        return ptr;
    }

private:
    NNamePtr _name_ptr;
    NStatementPtr _statement_ptr;
};

struct NAssignment : std::variant<NAssignmentV1>
{
    using std::variant<NAssignmentV1>::variant;
};
using NAssignmentPtr = std::shared_ptr<NAssignment>;

inline NAssignmentPtr parseAssignment(Lexer lexer)
{
    if (const auto name_ptr = parseName(lexer); name_ptr != nullptr)
    {
        if (getNextToken(lexer) != ":=")
        {
            throw std::invalid_argument { "expected := after name" };
        }

        skip(lexer, " \n");

        NAssignmentV1 node { name_ptr, parseStatement(lexer) };
        return std::make_shared<NAssignment>(std::move(node));
    }

    return nullptr;
}