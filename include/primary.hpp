#pragma once

#include "number.hpp"
#include "name.hpp"
#include "statement.hpp"

class NStatement;
using NStatementPtr = std::shared_ptr<NStatement>;
NStatementPtr parseSubstatement(Lexer lexer);

struct NPrimary : std::variant<NNumberPtr, NNamePtr, NStatementPtr>
{
    using std::variant<NNumberPtr, NNamePtr, NStatementPtr>::variant;
};
using NPrimaryPtr = std::shared_ptr<NPrimary>;

inline NPrimaryPtr parsePrimary(Lexer lexer)
{
    if (const auto number_ptr = parseNumber(lexer); number_ptr != nullptr)
    {
        NPrimary node { number_ptr };
        return std::make_shared<NPrimary>(std::move(node));
    }
    if (const auto name_ptr = parseName(lexer); name_ptr != nullptr)
    {
        NPrimary node { name_ptr };
        return std::make_shared<NPrimary>(std::move(node));
    }
    if (const auto substatement_ptr = parseSubstatement(lexer); substatement_ptr != nullptr)
    {
        NPrimary node { substatement_ptr };
        return std::make_shared<NPrimary>(std::move(node));
    }

    return nullptr;
}

