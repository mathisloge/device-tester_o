#include "data-flow/nodes/operators/division.hpp"
#include "data-flow/slots/floating_slot.hpp"
namespace dt::df::operators
{
    class Division::Impl final
    {
    public:
        Impl()
            : in_a{0}, in_b{0}
        {
        }

        void calc()
        {
            if (std::abs(in_b) < std::numeric_limits<double>::epsilon())
            {
               result_slot->setValue(std::numeric_limits<double>::infinity());
            }
            const double res = in_a / in_b;
            result_slot->setValue(res);
        }

        double in_a;
        double in_b;
        std::shared_ptr<NumberSlot> result_slot;
    };

    Division::Division(const NodeId id,
                       const SlotId in_a,
                       const SlotId in_b,
                       const SlotId out_res)
        : BaseNode{id, kNodeKey, "division",
                   Slots{
                       std::make_shared<FloatingSlot>(in_a, SlotType::input, "dividend"),
                       std::make_shared<FloatingSlot>(in_b, SlotType::input, "divisor")},
                   Slots{std::make_shared<FloatingSlot>(out_res, SlotType::output, "result")}},
          impl_{new Impl{}}
    {
        impl_->result_slot = std::dynamic_pointer_cast<NumberSlot>(outputs()[0]);

        inputs()[0]->subscribe([this](const BaseSlot *slot) {
            auto in_slot = dynamic_cast<const NumberSlot *>(slot);
            if (in_slot)
            {
                impl_->in_a = in_slot->value();
                impl_->calc();
            }
        });
        inputs()[1]->subscribe([this](const BaseSlot *slot) {
            auto in_slot = dynamic_cast<const NumberSlot *>(slot);
            if (in_slot)
            {
                impl_->in_b = in_slot->value();
                impl_->calc();
            }
        });
    }

    Division::~Division()
    {
        delete impl_;
    }
} // namespace dt::df::operators
