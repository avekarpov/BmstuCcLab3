#pragma once

#include "lexer.hpp"
#include "tools.hpp"

OPERATION(Power);
OPERATION(Abs);
OPERATION(Not);

// TODO: remove, not used
GROUP_OPERATIONS(PanOperation, NPowerPtr, NAbsPtr, NNotPtr);

PARSE_OPERATION(Power, "**", " ");
PARSE_OPERATION(Abs, "abs", " ");
PARSE_OPERATION(Not, "not", " ");

// TODO: remove, not used
inline NPanOperationPtr parsePanOperation(Lexer lexer)
{
    PARSE_OPERTATION_VARIANT(Power, PanOperation);
    PARSE_OPERTATION_VARIANT(Abs, PanOperation);
    PARSE_OPERTATION_VARIANT(Not, PanOperation);

    return nullptr;
}
