#include "data-flow/nodes/operators/simple_op.hpp"
#include "data-flow/slots/floating_slot.hpp"
namespace dt::df::operators
{
    class SimpleOp::Impl final
    {
    public:
        Impl()
            : in_a{0}, in_b{0}
        {
        }

        void setResult(const double res)
        {
            result_slot->setValue(res);
        }

        double in_a;
        double in_b;
        std::shared_ptr<NumberSlot> result_slot;
    };

    SimpleOp::SimpleOp(const NodeId id,
                       const NodeKey &key,
                       const std::string &title,
                       const SlotId in_a, const std::string &in_a_name,
                       const SlotId in_b, const std::string &in_b_name,
                       const SlotId out_res, const std::string &result_name)
        : BaseNode{id, key, title,
                   Slots{
                       std::make_shared<FloatingSlot>(FloatingSlot::kKey, in_a, SlotType::input, in_a_name),
                       std::make_shared<FloatingSlot>(FloatingSlot::kKey, in_b, SlotType::input, in_b_name)},
                   Slots{std::make_shared<FloatingSlot>(FloatingSlot::kKey, out_res, SlotType::output, result_name)}},
          impl_{new Impl{}}
    {
        impl_->result_slot = std::dynamic_pointer_cast<NumberSlot>(outputs()[0]);

        inputs()[0]->subscribe([this](const BaseSlot *slot) {
            auto in_slot = dynamic_cast<const NumberSlot *>(slot);
            if (in_slot)
            {
                impl_->in_a = in_slot->value();
                impl_->setResult(calc(impl_->in_a, impl_->in_b));
            }
        });
        inputs()[1]->subscribe([this](const BaseSlot *slot) {
            auto in_slot = dynamic_cast<const NumberSlot *>(slot);
            if (in_slot)
            {
                impl_->in_b = in_slot->value();
                impl_->setResult(calc(impl_->in_a, impl_->in_b));
            }
        });
    }

    static Slots makeInputs(const nlohmann::json &json)
    {
        Slots slots;
        try
        {
            const auto &in_json = json["inputs"];
            for (const auto &in_slot_j : in_json)
            {
                slots.emplace_back(std::make_shared<FloatingSlot>(in_slot_j));
            }
        }
        catch (...)
        {
        }
        return slots;
    }
    static Slots makeOutput(const nlohmann::json &json)
    {
        Slots slots;
        try
        {
            const auto &out_json = json["outputs"];
            if (out_json.size() != 1)
            {
                //! \todo log warn
            }
            if (out_json.size() > 0)
                slots.emplace_back(std::make_shared<FloatingSlot>(out_json[0]));
        }
        catch (...)
        {
        }
        return slots;
    }
    SimpleOp::SimpleOp(const nlohmann::json &json)
        : BaseNode(json, makeInputs(json), makeOutput(json)),
          impl_{new Impl{}}
    {
        impl_->result_slot = std::dynamic_pointer_cast<NumberSlot>(outputs()[0]);

        inputs()[0]->subscribe([this](const BaseSlot *slot) {
            auto in_slot = dynamic_cast<const NumberSlot *>(slot);
            if (in_slot)
            {
                impl_->in_a = in_slot->value();
                impl_->setResult(calc(impl_->in_a, impl_->in_b));
            }
        });
        inputs()[1]->subscribe([this](const BaseSlot *slot) {
            auto in_slot = dynamic_cast<const NumberSlot *>(slot);
            if (in_slot)
            {
                impl_->in_b = in_slot->value();
                impl_->setResult(calc(impl_->in_a, impl_->in_b));
            }
        });
    }

    SimpleOp::~SimpleOp()
    {
        delete impl_;
    }
} // namespace dt::df::operators
