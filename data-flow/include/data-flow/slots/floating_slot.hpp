#pragma once
#include "core/number_slot.hpp"
#include "dataflow_export.h"
namespace dt::df
{
    class DATAFLOW_EXPORT FloatingSlot final : public NumberSlot
    {
    public:
        using NumberSlot::NumberSlot;
        double value() const override;
        void accept(double value) override;
        ~FloatingSlot() = default;

    private:
        bool renderField() override;
        void renderValue() override;

    private:
        double value_ = 0;
    };
} // namespace dt::df
