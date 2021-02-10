#pragma once
#include "base_node.hpp"
#include "dataflow_export.h"
namespace dt::df
{
    class DATAFLOW_EXPORT TimerNode final : public BaseNode
    {
    public:
        static constexpr const char *kNodeKey = "timer-node";

    public:
        explicit TimerNode(const NodeId id,
                           const SlotId input_timer_id,
                           const SlotId output_trigger_id);
        const NodeKey &key() const override;
        ~TimerNode();

    private:
        void renderCustomContent() override;

    private:
        class Impl;
        Impl *impl_;
    };
} // namespace dt::df
