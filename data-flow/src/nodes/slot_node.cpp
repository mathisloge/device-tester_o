#include "data-flow/nodes/slot_node.hpp"

namespace dt::df
{
    class SlotNode::Impl final
    {
    public:
        explicit Impl(const std::string &key)
            : key_{key}
        {
        }
        const std::string key_;
    };

    SlotNode::SlotNode(const NodeId id, const std::string &key, const std::string &title, Slots &&outputs)
        : BaseNode{id, title, {}, std::forward<Slots>(outputs)},
          impl_{new Impl{key}}
    {
    }

    const NodeKey &SlotNode::key() const
    {
        return impl_->key_;
    }
} // namespace dt::df
