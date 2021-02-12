#pragma once
#include "simple_op.hpp"
#include "simple_op_builder.hpp"

namespace dt::df::operators
{
    DT_DF_BUILD_SIMPLE_OP(Division)
    DT_DF_BUILD_SIMPLE_OP(Multiplication)
    DT_DF_BUILD_SIMPLE_OP(Addition)
    DT_DF_BUILD_SIMPLE_OP(Subtraction)
} // namespace dt::df::operators
