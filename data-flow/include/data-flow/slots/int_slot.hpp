#pragma once
#include "base_slot.hpp"
#include "dataflow_export.h"
namespace dt::df
{
    class DATAFLOW_EXPORT IntSlot final : public BaseSlot
    {
    public:
        explicit IntSlot(const SlotId id, const SlotType type);
        void accept(const BaseSlot *slot) override;
        bool canConnect(const BaseSlot *const slot) const override;
        int value() const;
        void render() override;
        ~IntSlot() = default;

    private:
        int value_;
    };
} // namespace dt::df
