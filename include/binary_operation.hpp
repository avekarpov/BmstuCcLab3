#pragma once

#include "lexer.hpp"
#include "tools.hpp"

OPERATION(BiPlus);
OPERATION(BiMinus);
OPERATION(BiAnd);

GROUP_OPERATIONS(BiOperation, NBiPlusPtr, NBiMinusPtr, NBiAndPtr);

PARSE_OPERATION(BiPlus, "+", " #@(");
PARSE_OPERATION(BiMinus, "-", " #@(");
PARSE_OPERATION(BiAnd, "&", " #@(");

inline NBiOperationPtr parseBiOperation(Lexer lexer)
{
    PARSE_OPERTATION_VARIANT(BiPlus, BiOperation);
    PARSE_OPERTATION_VARIANT(BiMinus, BiOperation);
    PARSE_OPERTATION_VARIANT(BiAnd, BiOperation);

    return nullptr;
}
