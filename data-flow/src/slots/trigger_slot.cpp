#pragma once
#include "data-flow/slots/trigger_slot.hpp"
#include <imgui.h>
namespace dt::df
{
    TriggerSlot::TriggerSlot(const SlotId id,
                             const SlotType type,
                             const SlotName &name,
                             const SlotId local_id,
                             SlotFieldVisibility visibility_rule)
        : BaseSlot{kKey, id, type, name, local_id, visibility_rule}
    {
    }

    void TriggerSlot::trigger()
    {
        needsReevaluation();
    }

    void TriggerSlot::accept(const BaseSlot *slot)
    {
        auto input_slot = dynamic_cast<const TriggerSlot *>(slot);
        if (input_slot)
        {
            needsReevaluation();
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
            if (ImGui::SmallButton(name().c_str()))
                needsReevaluation();
        }
        else
        {
            ImGui::Text(name().c_str());
        }
    }
} // namespace dt::df
