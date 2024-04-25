#pragma once

#include "lexer.hpp"

class NNameV1
{
public:
    NNameV1(const std::string_view value)
    : _value { cast(value) }
    {}

    const std::string &value() const
    {
        return _value;
    }

private:
    static std::string cast(const std::string_view value)
    {
        if (not std::isalpha(value[0]))
        {
            throw std::invalid_argument { "expected name first letter" };
        }

        for (size_t i = 1; i < value.size(); ++i)
        {
            if (not std::isalnum(value[i]) && value[i] != '_')
            {
                throw std::invalid_argument { "expected name consisting of letters, digits or _" };
            }
        }

        return std::string { value };
    }

private:
    const std::string _value;
};

using NName = std::variant<NNameV1>;
using NNamePtr = std::shared_ptr<NName>;

inline NNamePtr parseName(Lexer lexer)
{    
    if (lexer.peekNext() != '@')
    {
        return nullptr;
    }
    lexer.next();

    const auto node = std::make_shared<NName>(getNextToken(lexer, " );"));
    skip(lexer, " \n");
    return node;
}
