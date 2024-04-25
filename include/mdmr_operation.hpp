#pragma once

#include "lexer.hpp"
#include "tools.hpp"

OPERATION(Mul);
OPERATION(Div);
OPERATION(Mod);
OPERATION(Rem);

GROUP_OPERATIONS(MdmrOperation, NMulPtr, NDivPtr, NModPtr, NRemPtr);

PARSE_OPERATION(Mul, "*", " ");
PARSE_OPERATION(Div, "/", " ");
PARSE_OPERATION(Mod, "mod", " ");
PARSE_OPERATION(Rem, "rem", " ");

inline NMdmrOperationPtr parseMdmrOperation(Lexer lexer)
{
    PARSE_OPERTATION_VARIANT(Mul, MdmrOperation);
    PARSE_OPERTATION_VARIANT(Div, MdmrOperation);
    PARSE_OPERTATION_VARIANT(Mod, MdmrOperation);
    PARSE_OPERTATION_VARIANT(Rem, MdmrOperation);

    return nullptr;
}
