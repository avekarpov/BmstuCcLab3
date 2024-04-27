#pragma once

#include "relation.hpp"
#include "logical_operation.hpp"

class NRelation;
using NRelationPtr = std::shared_ptr<NRelation>;
NRelationPtr parseRelation(Lexer lexer);

class NStatementV1
{
private:
    class Chunk
    {
    public:
        friend class NStatementV1;

        NLogicalOperationPtr logicalOperation() const
        {
            return _logical_operation_ptr;
        }

        NRelationPtr relation() const
        {
            return _relation_ptr;
        }

    private:
        NLogicalOperationPtr _logical_operation_ptr;
        NRelationPtr _relation_ptr;
    };

public:
    NStatementV1(NRelationPtr relation_ptr)
    : _relation_ptr { validate(relation_ptr) }
    {}

    void addChunk(NLogicalOperationPtr logical_operation_ptr, NRelationPtr relation_ptr)
    {
        _tail.emplace_back();
        _tail.back()._logical_operation_ptr = validate(logical_operation_ptr);
        _tail.back()._relation_ptr = validate(relation_ptr);
    }

    NRelationPtr relation() const
    {
        return _relation_ptr;
    }

    const std::vector<Chunk> &tail() const
    {
        return _tail;
    }

private:
    static NRelationPtr validate(NRelationPtr relation_ptr)
    {
        if (relation_ptr == nullptr)
        {
            throw std::runtime_error { "expected not null relation node" };
        }

        return relation_ptr;
    }

    static NLogicalOperationPtr validate(NLogicalOperationPtr logical_operation_ptr)
    {
        if (logical_operation_ptr == nullptr)
        {
            throw std::runtime_error { "expected not null logical operation node" };
        }

        return logical_operation_ptr;
    }

private:
    NRelationPtr _relation_ptr;
    std::vector<Chunk> _tail;
};

struct NStatement : std::variant<NStatementV1>
{
    using std::variant<NStatementV1>::variant;
};
using NStatementPtr = std::shared_ptr<NStatement>;

inline NStatementPtr parseStatement(Lexer lexer)
{
    NStatementV1 node { parseRelation(lexer) };
    for (
        auto logical_opertaion_ptr = parseLogicalOperation(lexer);
        logical_opertaion_ptr != nullptr;
        logical_opertaion_ptr = parseLogicalOperation(lexer)
    ) {
        node.addChunk(logical_opertaion_ptr, parseRelation(lexer));
    }

    return std::make_shared<NStatement>(std::move(node));
}

inline NStatementPtr parseSubstatement(Lexer lexer)
{
    if (lexer.peekNext() != '(')
    {
        return nullptr;
    }
    lexer.next();

    skip(lexer, " \n");

    const auto node = parseStatement(lexer);

    if (lexer.peekNext() != ')')
    {
        throw std::invalid_argument { 
            std::format("expected \")\" at {}, got {}", lexer.pos(), lexer.peekNext())
        };
    }
    lexer.next();

    skip(lexer, " \n");

    return node;
}
