#pragma once
#include "base_slot.hpp"
#include "dataflow_export.h"
namespace dt::df
{
    class DATAFLOW_EXPORT TriggerSlot final : public BaseSlot
    {
    public:
        explicit TriggerSlot(const SlotId id, const SlotType type);
        void accept(const BaseSlot *slot) override;
        bool canConnect(const BaseSlot *const slot) const override;
        void render() override;
        ~TriggerSlot() = default;
    };
} // namespace dt::df
