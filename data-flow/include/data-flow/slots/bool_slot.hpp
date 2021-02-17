#pragma once
#include "core/number_slot.hpp"
#include "dataflow_export.h"
namespace dt::df
{
    class DATAFLOW_EXPORT BoolSlot final : public NumberSlot
    {
    public:
        static constexpr std::string_view kKey = "BoolSlot";

    public:
        using NumberSlot::NumberSlot;
        void setValue(bool value);
        double value() const override;
        bool valueBool() const;
        ~BoolSlot() = default;

    private:
        void accept(double value) override;
        bool renderField() override;
        void renderValue() override;

    private:
        bool value_ = false;
    };
} // namespace dt::df
