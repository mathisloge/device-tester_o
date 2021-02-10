#include "data-flow/slots/base_slot.hpp"

namespace dt::df
{
    class BaseSlot::Impl
    {
    public:
        explicit Impl(const SlotId id)
            : id_{id}
        {
        }
        ~Impl()
        {
        }

    private:
        const SlotId id_;
        ValueChangedSignal value_changed_sig_;

        friend BaseSlot;
    };

    BaseSlot::BaseSlot(const SlotId id)
        : impl_{new BaseSlot::Impl{id}}
    {
    }

    SlotId BaseSlot::id() const
    {
        return impl_->id_;
    }

    boost::signals2::connection BaseSlot::subscribe(const ValueChangedSignal::slot_type &sub)
    {
        return impl_->value_changed_sig_.connect(sub);
    }

    void BaseSlot::valueChanged()
    {
        impl_->value_changed_sig_();
    }

    BaseSlot::~BaseSlot()
    {
        delete impl_;
    }

} // namespace dt::df
