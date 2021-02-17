#pragma once
#define DT_DF_BUILD_SIMPLE_OP(OP_NAME)                               \
    class DATAFLOW_EXPORT OP_NAME final : public SimpleOp            \
    {                                                                \
    public:                                                          \
        static constexpr const char *kNodeKey = #OP_NAME "-op-node"; \
        static constexpr const char *kNodeName = #OP_NAME " Node";   \
        OP_NAME(const NodeId id,                                     \
                const SlotId in_a,                                   \
                const SlotId in_b,                                   \
                const SlotId out_res);                               \
        OP_NAME(const nlohmann::json &);                             \
        ~##OP_NAME();                                                \
                                                                     \
    private:                                                         \
        double calc(const double a, const double b) const override;  \
    };

#define DT_DF_BUILD_SIMPLE_CMP(OP_NAME)                               \
    class DATAFLOW_EXPORT OP_NAME final : public SimpleCmp            \
    {                                                                 \
    public:                                                           \
        static constexpr const char *kNodeKey = #OP_NAME "-cmp-node"; \
        static constexpr const char *kNodeName = #OP_NAME " Node";    \
        OP_NAME(const NodeId id,                                      \
                const SlotId in_a,                                    \
                const SlotId in_b,                                    \
                const SlotId out_res,                                 \
                const SlotId true_res,                                \
                const SlotId false_res);                               \
        OP_NAME(const nlohmann::json &);                              \
        ~##OP_NAME();                                                 \
                                                                      \
    private:                                                          \
        bool cmp(const double a, const double b) const override;      \
    };
