#include "data-flow/nodes/base_node.hpp"

namespace dt::df
{

    class BaseNode::Impl
    {
    public:
        explicit Impl(const NodeId id,
                      Slots &&inputs,
                      Slots &&outputs)
            : id_{id},
              inputs_{std::move(inputs)},
              outputs_{std::move(outputs)}
        {
        }

    private:
        const NodeId id_;
        const Slots inputs_;
        const Slots outputs_;

        friend BaseNode;
    };

    BaseNode::BaseNode(const NodeId id,
                       Slots &&inputs,
                       Slots &&outputs)
        : impl_{new BaseNode::Impl{id, std::forward<Slots>(inputs), std::forward<Slots>(outputs)}}
    {
    }

    NodeId BaseNode::id() const
    {
        return impl_->id_;
    }

    const Slots &BaseNode::inputs() const
    {
        return impl_->inputs_;
    }
    const Slots &BaseNode::outputs() const
    {
        return impl_->outputs_;
    }

    const SlotPtr &BaseNode::inputs(const SlotId id) const
    {
        auto slot_it = std::find_if(impl_->inputs_.begin(), impl_->inputs_.end(), [id](const auto &slot) {
            return slot->id() == id;
        });

        if (slot_it == impl_->inputs_.end())
            return nullptr;
        return *slot_it;
    }
    const SlotPtr &BaseNode::outputs(const SlotId id) const
    {
        auto slot_it = std::find_if(impl_->outputs_.begin(), impl_->outputs_.end(), [id](const auto &slot) {
            return slot->id() == id;
        });

        if (slot_it == impl_->inputs_.end())
            return nullptr;
        return *slot_it;
    }

    BaseNode::~BaseNode()
    {
        delete impl_;
    }
} // namespace dt::df
