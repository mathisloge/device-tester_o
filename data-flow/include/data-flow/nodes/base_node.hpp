#pragma once
#include "node_types.hpp"
namespace dt::df
{
    class BaseNode
    {
    public:
        explicit BaseNode(const NodeId id,
                          Slots &&inputs,
                          Slots &&outputs);
        virtual const NodeKey &key() const = 0;

        NodeId id() const;
        const Slots &inputs() const;
        const Slots &outputs() const;
        const SlotPtr &inputs(const SlotId id) const;
        const SlotPtr &outputs(const SlotId id) const;
        virtual ~BaseNode();

    private:
        class Impl;
        Impl *impl_;
    };
} // namespace dt::df
