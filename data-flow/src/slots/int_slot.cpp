#pragma once
#include "data-flow/slots/int_slot.hpp"
#include <imgui.h>

namespace dt::df
{

    double IntSlot::value() const
    {
        return value_;
    }

    int IntSlot::valueInt() const
    {
        return value_;
    }

    void IntSlot::accept(double value)
    {
        value_ = static_cast<int>(value);
    }
    bool IntSlot::renderField()
    {
        return ImGui::InputInt(name().c_str(), &value_);
    }
    void IntSlot::renderValue()
    {
        ImGui::Value(name().c_str(), value_);
    }
} // namespace dt::df
