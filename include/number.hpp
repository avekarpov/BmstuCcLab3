#pragma once

#include "lexer.hpp"

class NNumberV1
{
public:
    NNumberV1(std::string_view value)
    : _value { cast(value) }
    {}

    unsigned long value() const
    {
        return _value;
    }

private:
    unsigned long cast(const std::string_view value)
    {
        for (size_t i = 0; i < value.size(); ++i)
        {
            if (not std::isdigit(value[i]))
            {
                throw std::invalid_argument { "expected number consisting of digits" };
            }
        }

        return std::stoul(std::string { value });
    }

private:
    const unsigned long _value;
};
using NNumber = std::variant<NNumberV1>;
using NNumberPtr = std::shared_ptr<NNumber>;

inline NNumberPtr parseNumber(Lexer lexer)
{
    if (lexer.peekNext() != '#')
    {
        return nullptr;
    }
    lexer.next();

    const auto node = std::make_shared<NNumber>(getNextToken(lexer, " );\n"));
    skip(lexer, " \n");
    return node;
}
