#pragma once
#include "slot_types.hpp"
#include <boost/signals2.hpp>
#include "dataflow_export.h"
namespace dt::df
{
    class DATAFLOW_EXPORT BaseSlot
    {
    public:
        using ValueChangedSignal = boost::signals2::signal<void()>;

    public:
        explicit BaseSlot(const SlotId id, const SlotType type);
        SlotId id() const;
        SlotType type() const;
        // will be called as an input
        virtual void accept(const BaseSlot *slot) = 0;
        virtual bool canConnect(const BaseSlot *const slot) const = 0;
        virtual void render();

        // will be called by the impl when an accept() was received or as an output from the node itself
        void valueChanged();
        boost::signals2::connection subscribe(const ValueChangedSignal::slot_type &sub);
        bool hasConnection() const;
        virtual ~BaseSlot();

    private:
        class Impl;
        Impl *impl_;
    };
} // namespace dt::df
