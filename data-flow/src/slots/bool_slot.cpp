#pragma once
#include "data-flow/slots/bool_slot.hpp"
#include <imgui.h>

namespace dt::df
{
    void BoolSlot::setValue(bool value)
    {
        value_ = value;
        needsReevaluation();
    }
    
    double BoolSlot::value() const
    {
        return value_;
    }

    bool BoolSlot::valueBool() const
    {
        return value_;
    }

    void BoolSlot::accept(double value)
    {
        value_ = static_cast<bool>(value);
    }

    bool BoolSlot::renderField()
    {
        return ImGui::Checkbox(name().c_str(), &value_);
    }

    void BoolSlot::renderValue()
    {
        ImGui::Value(name().c_str(), value_);
    }
} // namespace dt::df
