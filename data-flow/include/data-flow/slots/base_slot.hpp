#pragma once
#include "slot_types.hpp"
#include <boost/signals2.hpp>
namespace dt::df
{
    class BaseSlot
    {
    public:
        using ValueChangedSignal = boost::signals2::signal<void()>;

    public:
        explicit BaseSlot(const SlotId id);
        SlotId id() const;
        // will be called as an input
        virtual void accept(const BaseSlot *slot) = 0;
        virtual bool canConnect(const BaseSlot *const slot) const = 0;

        // will be called by the impl when an accept() was received or as an output from the node itself
        void valueChanged();
        boost::signals2::connection subscribe(const ValueChangedSignal::slot_type &sub);

        virtual ~BaseSlot();

    private:
        class Impl;
        Impl *impl_;
    };
} // namespace dt::df
