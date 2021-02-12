#pragma once
#include "data-flow/slots/floating_slot.hpp"
#include <imgui.h>

namespace dt::df
{

    double FloatingSlot::value() const
    {
        return value_;
    }

    void FloatingSlot::accept(double value)
    {
        value_ = value;
    }

    bool FloatingSlot::renderField()
    {
        return ImGui::InputDouble(name().c_str(), &value_);
    }
    
    void FloatingSlot::renderValue()
    {
        ImGui::Value(name().c_str(), static_cast<float>(value_));
    }
} // namespace dt::df
