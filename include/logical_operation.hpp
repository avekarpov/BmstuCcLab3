#pragma once

#include "lexer.hpp"
#include "tools.hpp"

OPERATION(And);
OPERATION(Or);
OPERATION(Xor);

GROUP_OPERATIONS(LogicalOperation, NAndPtr, NOrPtr, NXorPtr);

PARSE_OPERATION(And, "and", " ");
PARSE_OPERATION(Or, "or", " ");
PARSE_OPERATION(Xor, "xor", " ");

inline NLogicalOperationPtr parseLogicalOperation(Lexer lexer)
{
    PARSE_OPERTATION_VARIANT(And, LogicalOperation);
    PARSE_OPERTATION_VARIANT(Or, LogicalOperation);
    PARSE_OPERTATION_VARIANT(Xor, LogicalOperation);

    return nullptr;
}
