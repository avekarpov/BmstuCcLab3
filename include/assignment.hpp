#pragma once

#include "statement.hpp"
#include "block.hpp"

struct NBlock;
using NBlockPtr = std::shared_ptr<NBlock>;
NBlockPtr parseBlock(Lexer lexer);

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

class NAssignmentV2
{
public:
    NAssignmentV2(NBlockPtr block_ptr)
    : _block_ptr { validate(block_ptr) }
    {}

    NBlockPtr block() const
    {
        return _block_ptr;
    }

private:
    static NBlockPtr validate(NBlockPtr ptr)
    {
        if (ptr == nullptr)
        {
            throw std::runtime_error { "expected not null block node" };
        }
        
        return ptr;
    }

private:
    NBlockPtr _block_ptr;
};  

struct NAssignment : std::variant<NAssignmentV1, NAssignmentV2>
{
    using std::variant<NAssignmentV1, NAssignmentV2>::variant;
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
    if (const auto block_ptr = parseBlock(lexer); block_ptr != nullptr)
    {
        NAssignmentV2 node { block_ptr };
        return std::make_shared<NAssignment>(std::move(node));
    }

    return nullptr;
}