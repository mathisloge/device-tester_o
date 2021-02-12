#include "data-flow/slots/base_slot.hpp"

namespace dt::df
{
    class BaseSlot::Impl final
    {
    public:
        explicit Impl(const SlotId id, const SlotType type, SlotFieldVisibility visibility_rule)
            : id_{id}, type_{type}, visibility_rule_{visibility_rule}
        {
        }
        ~Impl()
        {
        }

    private:
        const SlotId id_;
        const SlotType type_;
        SlotFieldVisibility visibility_rule_;
        ValueChangedSignal value_changed_sig_;
        EvaluationSignal evaluation_changed_sig_;

        friend BaseSlot;
    };

    BaseSlot::BaseSlot(const SlotId id, const SlotType type, SlotFieldVisibility visibility_rule)
        : impl_{new BaseSlot::Impl{id, type, visibility_rule}}
    {
    }

    SlotId BaseSlot::id() const
    {
        return impl_->id_;
    }

    SlotType BaseSlot::type() const
    {
        return impl_->type_;
    }

    boost::signals2::connection BaseSlot::subscribe(const ValueChangedSignal::slot_type &sub)
    {
        return impl_->value_changed_sig_.connect(sub);
    }

    void BaseSlot::valueChanged()
    {
        impl_->value_changed_sig_();
    }

    bool BaseSlot::hasConnection() const
    {
        // when type_ == input, we got one connection to the node vertex
        return impl_->value_changed_sig_.num_slots() > 1;
    }

    void BaseSlot::render()
    {
    }

    SlotFieldVisibility BaseSlot::visibility_rule() const
    {
        return impl_->visibility_rule_;
    }
    void BaseSlot::visibility_rule(SlotFieldVisibility visibility_rule)
    {
        impl_->visibility_rule_ = visibility_rule;
    }

    bool BaseSlot::showField() const
    {
        return impl_->visibility_rule_ == SlotFieldVisibility::always ||
               !(hasConnection() && impl_->visibility_rule_ == SlotFieldVisibility::without_connection);
    }

    boost::signals2::connection BaseSlot::connectEvaluation(const EvaluationSignal::slot_type &sub)
    {
        return impl_->evaluation_changed_sig_.connect(sub);
    }

    void BaseSlot::needsReevaluation()
    {
        impl_->evaluation_changed_sig_(impl_->id_);
    }

    BaseSlot::~BaseSlot()
    {
        delete impl_;
    }

} // namespace dt::df
