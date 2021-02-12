#pragma once
#include "base_slot.hpp"
namespace dt::df
{
    class DATAFLOW_EXPORT NumberSlot : public BaseSlot
    {
    public:
        using BaseSlot::BaseSlot;
        virtual void setValue(double value);
        virtual double value() const = 0;
        virtual void accept(const BaseSlot *slot) override;
        virtual bool canConnect(const BaseSlot *const slot) const override;
        virtual void render() override;
        virtual ~NumberSlot();

    protected:
        virtual void accept(double value) = 0;
        virtual bool renderField() = 0;
        virtual void renderValue() = 0;
    };
} // namespace dt::df
