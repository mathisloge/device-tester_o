#pragma once
#define DT_DF_BUILD_SIMPLE_OP(OP_NAME)                               \
    class DATAFLOW_EXPORT OP_NAME final : public SimpleOp            \
    {                                                                \
    public:                                                          \
        static constexpr const char *kNodeKey = #OP_NAME "-op-node"; \
        explicit OP_NAME(const NodeId id,                            \
                         const SlotId in_a,                          \
                         const SlotId in_b,                          \
                         const SlotId out_res);                      \
        ~##OP_NAME();                                                \
                                                                     \
    private:                                                         \
        double calc(const double a, const double b) const override;  \
    };
