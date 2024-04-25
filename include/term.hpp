#pragma once

#include "factor.hpp"
#include "mdmr_operation.hpp"

class NFactor;
using NFactorPtr = std::shared_ptr<NFactor>;
NFactorPtr parseFactor(Lexer lexer);

class NTermV1
{
private:
    class Chunk
    {
    public:
        friend class NTermV1;

        NMdmrOperationPtr mdmrOperation() const
        {
            return _mdmr_operation_ptr;
        }

        NFactorPtr factor() const
        {
            return _factor_ptr;
        }

    private:
        NMdmrOperationPtr _mdmr_operation_ptr;
        NFactorPtr _factor_ptr;
    };

public:
    NTermV1(NFactorPtr factor_ptr)
    : _factor_ptr { factor_ptr }
    {}

    void addChunk(NMdmrOperationPtr mdmr_operation_ptr, NFactorPtr factor_ptr)
    {
        _tail.emplace_back();
        _tail.back()._mdmr_operation_ptr = validate(mdmr_operation_ptr);
        _tail.back()._factor_ptr = validate(factor_ptr);
    }

    NFactorPtr factor() const
    {
        return _factor_ptr;
    }

    const std::vector<Chunk> &tail() const
    {
        return _tail;
    }    

private:
    static NFactorPtr validate(NFactorPtr factor_ptr)
    {
        if (factor_ptr == nullptr)
        {
            throw std::runtime_error { "expected not null factor node" };
        }

        return factor_ptr;
    }

    static NMdmrOperationPtr validate(NMdmrOperationPtr mdmr_operation_ptr)
    {
        if (mdmr_operation_ptr == nullptr)
        {
            throw std::runtime_error { "expected not null mdmr operation node" };
        }

        return mdmr_operation_ptr;
    }

private:
    NFactorPtr _factor_ptr;
    std::vector<Chunk> _tail;
};

struct NTerm : public std::variant<NTermV1>
{
    using std::variant<NTermV1>::variant;
};
using NTermPtr = std::shared_ptr<NTerm>;

inline NTermPtr parseTerm(Lexer lexer)
{
    NTermV1 node { parseFactor(lexer) };
    for (auto mdmr_operation_ptr = parseMdmrOperation(lexer); mdmr_operation_ptr != nullptr; mdmr_operation_ptr = parseMdmrOperation(lexer))
    {
        node.addChunk(mdmr_operation_ptr, parseFactor(lexer));
    }
    return std::make_shared<NTerm>(std::move(node));
}
