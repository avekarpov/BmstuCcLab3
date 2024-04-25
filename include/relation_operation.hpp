#pragma once

#include "lexer.hpp"
#include "tools.hpp"

OPERATION(Low);
OPERATION(LowEq);
OPERATION(Eq);
OPERATION(NotEq);
OPERATION(Great);
OPERATION(GreatEq);

GROUP_OPERATIONS(
    RelationOperation, 
    NLowPtr, NLowEqPtr, 
    NEqPtr, NNotEqPtr, 
    NGreatPtr, NGreatEqPtr
);

PARSE_OPERATION(Low, "<", " ");
PARSE_OPERATION(LowEq, "<=", " ");
PARSE_OPERATION(Eq, "=", " ");
PARSE_OPERATION(NotEq, "/>", " ");
PARSE_OPERATION(Great, ">", " ");
PARSE_OPERATION(GreatEq, ">=", " ");

inline NRelationOperationPtr parseRelationOperation(Lexer lexer)
{
    PARSE_OPERTATION_VARIANT(Low, RelationOperation);
    PARSE_OPERTATION_VARIANT(LowEq, RelationOperation);
    PARSE_OPERTATION_VARIANT(Eq, RelationOperation);
    PARSE_OPERTATION_VARIANT(NotEq, RelationOperation);
    PARSE_OPERTATION_VARIANT(Great, RelationOperation);
    PARSE_OPERTATION_VARIANT(GreatEq, RelationOperation);

    return nullptr;
}
