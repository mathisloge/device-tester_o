#pragma once

#define DT_DF_IMPL_SIMPLE_OP_BEGIN(OP_NAME, TITLE, NAME_A, NAME_B, NAME_RES)  \
    OP_NAME::##OP_NAME(const NodeId id,                                       \
                       const SlotId in_a,                                     \
                       const SlotId in_b,                                     \
                       const SlotId out_res) : SimpleOp{id, kNodeKey, #TITLE, \
                                                        in_a, #NAME_A,        \
                                                        in_b, #NAME_B,        \
                                                        out_res, #NAME_RES}   \
    {                                                                         \
    }                                                                         \
                                                                              \
    OP_NAME::##OP_NAME(const nlohmann::json &json)                                       \
        : SimpleOp{json}                                                      \
    {                                                                         \
    }                                                                         \
                                                                              \
    OP_NAME::~##OP_NAME()                                                     \
    {                                                                         \
    }                                                                         \
    double OP_NAME::calc(const double a, const double b) const                \
    {

#define DT_DF_IMPL_SIMPLE_OP_END }
