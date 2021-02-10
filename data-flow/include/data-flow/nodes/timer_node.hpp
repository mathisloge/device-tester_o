#pragma once
#include "base_node.hpp"
namespace dt::df
{
    class TimerNode final : public BaseNode
    {
    public:
        explicit TimerNode(const NodeId id,
                           const SlotId input_timer_id,
                           const SlotId output_trigger_id);
        ~TimerNode();

    private:
        class Impl;
        Impl *impl_;
    };
} // namespace dt::df
