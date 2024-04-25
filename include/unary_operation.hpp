#pragma once

#include "lexer.hpp"

// TODO: use tools

struct NUnPlusV1 {};

using NUnPlus = std::variant<NUnPlusV1>;
using NUnPlusPtr = std::shared_ptr<NUnPlus>;

struct NUnMinusV1 {};

using NUnMinus = std::variant<NUnMinusV1>;
using NUnMinusPtr = std::shared_ptr<NUnMinus>;

using NUnOperation = std::variant<NUnPlusPtr, NUnMinusPtr>;
using NUnOperationPtr = std::shared_ptr<NUnOperation>;

inline NUnPlusPtr parseUnPlus(Lexer lexer)
{
    static constexpr std::string_view token = "+";

    if (peekNextToken(lexer, " #@(") != token)
    {
        return nullptr;
    }
    next(lexer, token.size());

    skip(lexer, " \n");

    return std::make_shared<NUnPlus>();
}

inline NUnMinusPtr parseUnMinus(Lexer lexer)
{
    static constexpr std::string_view token = "-";

    if (peekNextToken(lexer, " #@(") != token)
    {
        return nullptr;
    }
    next(lexer, token.size());

    skip(lexer, " \n");

    return std::make_shared<NUnMinus>();
}

inline NUnOperationPtr parseUnOperation(Lexer lexer)
{
    if (const auto un_plus_ptr = parseUnPlus(lexer); un_plus_ptr != nullptr)
    {
        NUnOperation node { un_plus_ptr };
        return std::make_shared<NUnOperation>(std::move(node));
    }
    if (const auto un_minus_ptr = parseUnMinus(lexer); un_minus_ptr != nullptr)
    {
        NUnOperation node { un_minus_ptr };
        return std::make_shared<NUnOperation>(std::move(node));
    }

    return nullptr;
}