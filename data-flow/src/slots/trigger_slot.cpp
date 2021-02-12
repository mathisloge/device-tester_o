#pragma once
#include "data-flow/slots/trigger_slot.hpp"
#include <imgui.h>
namespace dt::df
{
    TriggerSlot::TriggerSlot(const SlotId id, const SlotType type, SlotFieldVisibility visibility_rule)
        : BaseSlot{id, type, visibility_rule}
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
        if (showField())
        {
            if (ImGui::SmallButton("trigger"))
                needsReevaluation();
        }
        else
        {
            ImGui::Text("trigger");
        }
    }
} // namespace dt::df
