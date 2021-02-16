#pragma once
#include "core/base_slot.hpp"
#include "dataflow_export.h"
namespace dt::df
{
    class DATAFLOW_EXPORT TriggerSlot final : public BaseSlot
    {
    public:
        static constexpr std::string_view kKey = "TriggerSlot";

    public:
        using BaseSlot::BaseSlot;
        explicit TriggerSlot(const SlotId id,
                             const SlotType type,
                             const SlotName &name = "trigger",
                             const SlotId local_id = 0,
                             SlotFieldVisibility visibility_rule = SlotFieldVisibility::without_connection);
        void accept(const BaseSlot *slot) override;
        bool canConnect(const BaseSlot *const slot) const override;
        void render() override;
        void trigger();
        ~TriggerSlot() = default;
    };
} // namespace dt::df
