#pragma once
#include "slot_types.hpp"
#include <memory>
#include <boost/signals2.hpp>
#include "dataflow_export.h"
namespace dt::df
{
    class DATAFLOW_EXPORT BaseSlot
    {
    public:
        using ValueChangedSignal = boost::signals2::signal<void(const BaseSlot *slot)>;
        using EvaluationSignal = boost::signals2::signal<void(SlotId)>;

    public:
        explicit BaseSlot(const SlotId id,
                          const SlotType type,
                          const SlotName &name,
                          SlotFieldVisibility visibility_rule = SlotFieldVisibility::without_connection);
        SlotId id() const;
        SlotType type() const;
        const SlotName &name() const;
        virtual void connectEvent();
        virtual void disconnectEvent();
        // will be called as an input
        virtual void accept(const BaseSlot *slot) = 0;
        virtual bool canConnect(const BaseSlot *const slot) const = 0;
        virtual void render();

        void valueChanged();
        boost::signals2::connection subscribe(const ValueChangedSignal::slot_type &sub);
        bool hasConnection() const;
        SlotFieldVisibility visibility_rule() const;
        void visibility_rule(SlotFieldVisibility visibility_rule);
        virtual ~BaseSlot();

        boost::signals2::connection connectEvaluation(const EvaluationSignal::slot_type &sub);

    protected:
        void needsReevaluation();
        bool showField() const;

    private:
        class Impl;
        Impl *impl_;
    };
} // namespace dt::df