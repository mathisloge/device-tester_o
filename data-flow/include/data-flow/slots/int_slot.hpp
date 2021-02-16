#pragma once
#include "core/number_slot.hpp"
#include "dataflow_export.h"
namespace dt::df
{
    class DATAFLOW_EXPORT IntSlot final : public NumberSlot
    {
    public:
        static constexpr std::string_view kKey = "FloatingSlot";

    public:
        using NumberSlot::NumberSlot;
        double value() const override;
        int valueInt() const;
        ~IntSlot() = default;

    private:
        void accept(double value) override;
        bool renderField() override;
        void renderValue() override;

    private:
        int value_ = 0;
    };
} // namespace dt::df
