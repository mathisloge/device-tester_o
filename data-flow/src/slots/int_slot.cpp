#pragma once
#include "data-flow/slots/int_slot.hpp"

namespace dt::df
{
    IntSlot::IntSlot(const SlotId id)
        : BaseSlot{id},
          value_{0}
    {
    }

    void IntSlot::accept(const BaseSlot *slot)
    {
        auto input_slot = dynamic_cast<const IntSlot *>(slot);
        if (input_slot)
        {
            value_ = input_slot->value();
            valueChanged();
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
} // namespace dt::df
