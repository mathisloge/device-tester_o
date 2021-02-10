#pragma once
#include "data-flow/slots/trigger_slot.hpp"
#include <imgui.h>
namespace dt::df
{
    TriggerSlot::TriggerSlot(const SlotId id, const SlotType type)
        : BaseSlot{id, type}
    {
    }

    void TriggerSlot::accept(const BaseSlot *slot)
    {
        auto input_slot = dynamic_cast<const TriggerSlot *>(slot);
        if (input_slot)
        {
            valueChanged();
        }
    }

    bool TriggerSlot::canConnect(const BaseSlot *const slot) const
    {
        return dynamic_cast<const TriggerSlot *const>(slot);
    }

    void TriggerSlot::render()
    {
        ImGui::Text("trigger");
    }
} // namespace dt::df
