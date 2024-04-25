#pragma once

#include "simple_statement.hpp"
#include "relation_operation.hpp"

class NSimplestatement;
using NSimplestatementPtr = std::shared_ptr<NSimplestatement>;
NSimplestatementPtr parseSimpleStatement(Lexer lexer);

class NRelationV1
{
private:
    class Chunk
    {
    public:
        friend class NRelationV1;

        NRelationOperationPtr relationOperation() const
        {
            return _relation_operation_ptr;
        }

        NSimplestatementPtr simpleStatement() const
        {
            return _simple_statement_ptr;
        }

    private:
        NRelationOperationPtr _relation_operation_ptr;
        NSimplestatementPtr _simple_statement_ptr;
    };

public:
    NRelationV1(NSimplestatementPtr simple_statement_ptr)
    : _simple_statement_ptr { validate(simple_statement_ptr) }
    {}

    void addChunk(NRelationOperationPtr relation_operation_ptr, NSimplestatementPtr simple_statement_ptr)
    {
        _tail.emplace_back();
        _tail.back()._relation_operation_ptr = validate(relation_operation_ptr);
        _tail.back()._simple_statement_ptr = validate(simple_statement_ptr);
    }

    NSimplestatementPtr simpleStatement() const
    {
        return _simple_statement_ptr;
    }

    const std::vector<Chunk> &tail() const
    {
        return _tail;
    }

private:
    static NSimplestatementPtr validate(NSimplestatementPtr simple_statement_ptr)
    {
        if (simple_statement_ptr == nullptr)
        {
            throw std::runtime_error { "expected not null simple statement node" };
        }
        
        return simple_statement_ptr;
    }

    static NRelationOperationPtr validate(NRelationOperationPtr relation_operation_ptr)
    {
        if (relation_operation_ptr == nullptr)
        {
            throw std::runtime_error { "expected not null relation operation node" };
        }
        
        return relation_operation_ptr;
    }

private:
    NSimplestatementPtr _simple_statement_ptr;
    std::vector<Chunk> _tail;
};

struct NRelation : std::variant<NRelationV1>
{
    using std::variant<NRelationV1>::variant;
};
using NRelationPtr = std::shared_ptr<NRelation>;

inline NRelationPtr parseRelation(Lexer lexer)
{
    NRelationV1 node { parseSimpleStatement(lexer) };
    for (
        auto relation_operation_ptr = parseRelationOperation(lexer); 
        relation_operation_ptr != nullptr;
        relation_operation_ptr = parseRelationOperation(lexer)
    ) {
        node.addChunk(relation_operation_ptr, parseSimpleStatement(lexer));
    }
    return std::make_shared<NRelation>(std::move(node));
}