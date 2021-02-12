#pragma once
#include "data-flow/slots/int_slot.hpp"
#include <imgui.h>

namespace dt::df
{
    IntSlot::IntSlot(const SlotId id, const SlotType type, SlotFieldVisibility visibility_rule)
        : BaseSlot{id, type, visibility_rule},
          value_{0}
    {
    }

    void IntSlot::accept(const BaseSlot *slot)
    {
        auto input_slot = dynamic_cast<const IntSlot *>(slot);
        if (input_slot)
        {
            value_ = input_slot->value();
            needsReevaluation();
        }
    }

    bool IntSlot::canConnect(const BaseSlot *const slot) const
    {
        return dynamic_cast<const IntSlot *const>(slot);
    }

    int IntSlot::value() const
    {
        return value_;
    }

    void IntSlot::render()
    {
        if (showField())
        {
            ImGui::SetNextItemWidth(100);
            if (ImGui::InputInt("Value", &value_))
            {
                needsReevaluation();
            }
        } else {
            ImGui::Value("Value", value_);
        }
    }
} // namespace dt::df
