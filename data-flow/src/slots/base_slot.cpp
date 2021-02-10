#include "data-flow/slots/base_slot.hpp"

namespace dt::df
{
    class BaseSlot::Impl
    {
    public:
        explicit Impl(const SlotId id, const SlotType type)
            : id_{id}, type_{type}
        {
        }
        ~Impl()
        {
        }

    private:
        const SlotId id_;
        const SlotType type_;
        ValueChangedSignal value_changed_sig_;

        friend BaseSlot;
    };

    BaseSlot::BaseSlot(const SlotId id, const SlotType type)
        : impl_{new BaseSlot::Impl{id, type}}
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
        return impl_->value_changed_sig_.num_slots() > (impl_->type_ == SlotType::input ? 1 : 0);
    }

    void BaseSlot::render()
    {
    }

    BaseSlot::~BaseSlot()
    {
        delete impl_;
    }

} // namespace dt::df
