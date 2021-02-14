#include "data-flow/nodes/operators/standard_ops.hpp"
#include "simple_op_builder_impl.hpp"

namespace dt::df::operators
{

    DT_DF_IMPL_SIMPLE_OP_BEGIN(Division, division, dividend, divisor, result)
    return (std::abs(b) < std::numeric_limits<double>::epsilon())
               ? std::numeric_limits<double>::infinity()
               : a / b;
    DT_DF_IMPL_SIMPLE_OP_END

    DT_DF_IMPL_SIMPLE_OP_BEGIN(Multiplication, multiply, a, b, result)
    return a * b;
    DT_DF_IMPL_SIMPLE_OP_END

    DT_DF_IMPL_SIMPLE_OP_BEGIN(Addition, addition, a, b, result)
    return a + b;
    DT_DF_IMPL_SIMPLE_OP_END

    DT_DF_IMPL_SIMPLE_OP_BEGIN(Subtraction, subtraction, a, b, result)
    return a - b;
    DT_DF_IMPL_SIMPLE_OP_END

    DT_DF_IMPL_SIMPLE_OP_BEGIN(Modulo, modulo, a, n, result)
    return std::remainder(a, b);
    DT_DF_IMPL_SIMPLE_OP_END

} // namespace dt::df::operators
