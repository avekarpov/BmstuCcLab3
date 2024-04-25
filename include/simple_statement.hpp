#pragma once

#include "unary_operation.hpp"
#include "binary_operation.hpp"
#include "term.hpp"

class NTerm;
using NTermPtr = std::shared_ptr<NTerm>;
NTermPtr parseTerm(Lexer lexer);

class NSimplestatementV1
{
private:
    class Chunk
    {
    public:
        friend class NSimplestatementV1;

        NBiOperationPtr biOperation() const
        {
            return _bi_operation_ptr;
        }

        NTermPtr term() const
        {
            return _term_ptr;
        }

    private:
        NBiOperationPtr _bi_operation_ptr;
        NTermPtr _term_ptr;
    };

public:
    NSimplestatementV1(NUnOperationPtr un_operation_ptr, NTermPtr term_ptr)
    : _un_operation_ptr { validate(un_operation_ptr) }
    , _term_ptr { validate(term_ptr) }
    {}

    void addChunk(NBiOperationPtr bi_operation_ptr, NTermPtr term_ptr)
    {
        _tail.emplace_back();
        _tail.back()._bi_operation_ptr = validate(bi_operation_ptr);
        _tail.back()._term_ptr = validate(term_ptr);
    }

    NUnOperationPtr unOperation() const
    {
        return _un_operation_ptr;
    }

    NTermPtr term() const
    {
        return _term_ptr;
    }

    const std::vector<Chunk> &tail() const
    {
        return _tail;
    }

private:
    static NUnOperationPtr validate(NUnOperationPtr un_operation_ptr)
    {
        return un_operation_ptr;
    }

    static NTermPtr validate(NTermPtr term_ptr)
    {
        if (term_ptr == nullptr)
        {
            throw std::runtime_error { "expected not null term node" };
        }

        return term_ptr;
    }

    static NBiOperationPtr validate(NBiOperationPtr bi_operation_ptr)
    {
        if (bi_operation_ptr == nullptr)
        {
            throw std::runtime_error { "expected not null bi operation node" };
        }

        return bi_operation_ptr;
    }

private:
    NUnOperationPtr _un_operation_ptr;
    NTermPtr _term_ptr;
    std::vector<Chunk> _tail;
};

struct NSimplestatement : std::variant<NSimplestatementV1>
{
    using std::variant<NSimplestatementV1>::variant;
};
using NSimplestatementPtr = std::shared_ptr<NSimplestatement>;

inline NSimplestatementPtr parseSimpleStatement(Lexer lexer)
{
    NSimplestatementV1 node
    {
        parseUnOperation(lexer),
        parseTerm(lexer)
    };
    for (auto bi_operation_ptr = parseBiOperation(lexer); bi_operation_ptr != nullptr; bi_operation_ptr = parseBiOperation(lexer))
    {
        node.addChunk(bi_operation_ptr, parseTerm(lexer));
    }

    return std::make_shared<NSimplestatement>(std::move(node));
}
