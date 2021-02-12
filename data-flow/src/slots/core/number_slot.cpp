#include "data-flow/slots/core/number_slot.hpp"
#include <imgui.h>
namespace dt::df
{
    void NumberSlot::setValue(double value)
    {
        accept(value);
        needsReevaluation();
    }

    void NumberSlot::accept(const BaseSlot *slot)
    {
        auto input_slot = dynamic_cast<const NumberSlot *>(slot);
        if (input_slot)
        {
            accept(input_slot->value());
            needsReevaluation();
        }
    }

    bool NumberSlot::canConnect(const BaseSlot *const slot) const
    {
        return dynamic_cast<const NumberSlot *const>(slot);
    }

    void NumberSlot::render()
    {
        if (showField())
        {
            ImGui::SetNextItemWidth(100);
            if (renderField())
            {
                needsReevaluation();
            }
        }
        else
        {
            renderValue();
        }
    }

    NumberSlot::~NumberSlot()
    {
    }
} // namespace dt::df
