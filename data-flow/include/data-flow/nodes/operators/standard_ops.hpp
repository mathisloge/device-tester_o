#pragma once
#include "simple_op.hpp"
#include "simple_cmp.hpp"
#include "simple_op_builder.hpp"

namespace dt::df::operators
{
    DT_DF_BUILD_SIMPLE_OP(Division)
    DT_DF_BUILD_SIMPLE_OP(Multiplication)
    DT_DF_BUILD_SIMPLE_OP(Addition)
    DT_DF_BUILD_SIMPLE_OP(Subtraction)
    DT_DF_BUILD_SIMPLE_OP(Modulo)
    DT_DF_BUILD_SIMPLE_OP(Pow)

    DT_DF_BUILD_SIMPLE_CMP(Less)
    DT_DF_BUILD_SIMPLE_CMP(LEQ)
    DT_DF_BUILD_SIMPLE_CMP(EQ)
    DT_DF_BUILD_SIMPLE_CMP(NEQ)
    DT_DF_BUILD_SIMPLE_CMP(GEQ)
    DT_DF_BUILD_SIMPLE_CMP(Greater)
} // namespace dt::df::operators
