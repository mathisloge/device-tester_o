#pragma once
#include "../core/base_node.hpp"

namespace dt::df
{
    class DATAFLOW_EXPORT TimerNode final : public BaseNode
    {
    public:
        static constexpr const char *kNodeKey = "timer-node";

    public:
        TimerNode(const NodeId id,
                           const SlotId input_timer_id,
                           const SlotId output_trigger_id);
        TimerNode(const nlohmann::json &json);
        ~TimerNode();

    private:
        class Impl;
        Impl *impl_;
    };
} // namespace dt::df
