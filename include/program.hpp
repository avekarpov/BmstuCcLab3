#pragma once

#include "block.hpp"

class NProgramV1
{
public:
    NProgramV1(NBlockPtr block_ptr)
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

struct NProgram : std::variant<NProgramV1>
{
    using std::variant<NProgramV1>::variant;
};
using NProgramPtr = std::shared_ptr<NProgram>;

inline NProgramPtr parseProgram(Lexer lexer)
{
    NProgramV1 node { parseBlock(lexer) };
    return std::make_shared<NProgram>(std::move(node));
}