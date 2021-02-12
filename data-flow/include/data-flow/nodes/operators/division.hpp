#pragma once
#include "../core/base_node.hpp"

namespace dt::df::operators
{
    class DATAFLOW_EXPORT Division final : public BaseNode
    {
    public:
        static constexpr const char *kNodeKey = "division-op-node";

        explicit Division(const NodeId id,
                          const SlotId in_a,
                          const SlotId in_b,
                          const SlotId out_res);
        ~Division();

    private:
        class Impl;
        Impl *impl_;
    };
} // namespace dt::df::operators
