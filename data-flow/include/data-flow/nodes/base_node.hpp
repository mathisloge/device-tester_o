#pragma once
#include "node_types.hpp"
#include "dataflow_export.h"
namespace dt::df
{
    class DATAFLOW_EXPORT BaseNode
    {
    public:
        explicit BaseNode(const NodeId id,
                          const std::string &title,
                          Slots &&inputs,
                          Slots &&outputs);

        virtual void render();

        virtual const NodeKey &key() const = 0;
        NodeId id() const;
        const Slots &inputs() const;
        const Slots &outputs() const;
        const SlotPtr &inputs(const SlotId id) const;
        const SlotPtr &outputs(const SlotId id) const;
        virtual ~BaseNode();

    protected:
        virtual void renderCustomContent();

    private:
        class Impl;
        Impl *impl_;
    };
} // namespace dt::df
