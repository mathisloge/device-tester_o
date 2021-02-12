#pragma once
#include "base_slot.hpp"
#include "dataflow_export.h"
namespace dt::df
{
    class DATAFLOW_EXPORT TriggerSlot final : public BaseSlot
    {
    public:
        explicit TriggerSlot(const SlotId id, const SlotType type, SlotFieldVisibility visibility_rule = SlotFieldVisibility::without_connection);
        void accept(const BaseSlot *slot) override;
        bool canConnect(const BaseSlot *const slot) const override;
        void render() override;
        ~TriggerSlot() = default;
    };
} // namespace dt::df
