#pragma once
#include "base_slot.hpp"
namespace dt::df
{
    class IntSlot final : public BaseSlot
    {
    public:
        explicit IntSlot(const SlotId id);
        void accept(const BaseSlot *slot) override;
        bool canConnect(const BaseSlot *const slot) const override;
        int value() const;
        ~IntSlot() = default;

    private:
        int value_;
    };
} // namespace dt::df
