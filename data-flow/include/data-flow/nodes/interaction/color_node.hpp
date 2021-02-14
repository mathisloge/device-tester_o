#pragma once
#include "../core/base_node.hpp"

namespace dt::df
{
    class DATAFLOW_EXPORT ColorNode final : public BaseNode
    {
    public:
        static constexpr const char *kNodeKey = "color-node";

    public:
        ColorNode(const NodeId id,
                           const SlotId output_r,
                           const SlotId output_g,
                           const SlotId output_b,
                           const SlotId output_a);
        ColorNode(const nlohmann::json &json);
        ~ColorNode();

    private:
        void renderCustomContent() override;

    private:
        class Impl;
        Impl *impl_;
    };
} // namespace dt::df
