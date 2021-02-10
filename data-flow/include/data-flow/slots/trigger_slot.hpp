#pragma once
#include "base_slot.hpp"
namespace dt::df
{
    class TriggerSlot final : public BaseSlot
    {
    public:
        explicit TriggerSlot(const SlotId id);
        void accept(const BaseSlot *slot) override;
        bool canConnect(const BaseSlot *const slot) const override;
        ~TriggerSlot() = default;
    };
} // namespace dt::df
