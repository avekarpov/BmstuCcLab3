#pragma once

#include "list_assignment.hpp"

class NBlockV1
{
public:
    NBlockV1(NListAssignmentPtr list_assignment_ptr)
    : _list_assignment_ptr { validate(list_assignment_ptr) }
    {}

    NListAssignmentPtr listAssignment() const
    {
        return _list_assignment_ptr;
    }

private:
    static NListAssignmentPtr validate(NListAssignmentPtr ptr)
    {
        if (ptr == nullptr)
        {
            throw std::runtime_error { "expected not null list assignment node" };
        }
        
        return ptr;
    }

private:
    NListAssignmentPtr _list_assignment_ptr;
};

struct NBlock : std::variant<NBlockV1>
{
    using std::variant<NBlockV1>::variant;
};
using NBlockPtr = std::shared_ptr<NBlock>;

inline NBlockPtr parseBlock(Lexer lexer)
{
    if (lexer.peekNext() != '{')
    {
        return nullptr;
    }
    lexer.next();

    skip(lexer, " \n");

    NBlockV1 node { parseListAssignment(lexer) };

    if (lexer.peekNext() != '}')
    {
        throw std::invalid_argument { 
            std::format("expected \"}}\" at {}, got {}", lexer.pos(), lexer.peekNext())
        };
    }
    lexer.next();

    skip(lexer, " \n");
    
    return std::make_shared<NBlock>(std::move(node));
}