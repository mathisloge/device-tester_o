#pragma once
#include "../core/base_node.hpp"

namespace dt::df
{
    class DATAFLOW_EXPORT ColorNode final : public BaseNode
    {
    public:
        static constexpr const char *kNodeKey = "color-node";

    public:
        explicit ColorNode(const NodeId id,
                           const SlotId output_r,
                           const SlotId output_g,
                           const SlotId output_b,
                           const SlotId output_a);
        ~ColorNode();

    private:
        void renderCustomContent() override;

    private:
        class Impl;
        Impl *impl_;
    };
} // namespace dt::df
