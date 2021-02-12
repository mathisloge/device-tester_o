#pragma once
#include "base_node.hpp"
#include "../slots/base_slot.hpp"
#include "dataflow_export.h"
namespace dt::df
{
    /**
     * @brief this slot just has a output slots.
     * 
     */
    class DATAFLOW_EXPORT SlotNode final : public BaseNode
    {
    public:
        explicit SlotNode(const NodeId id, const std::string &key, const std::string &title, Slots &&outputs);
        const NodeKey &key() const override;

    private:
        class Impl;
        Impl *impl_;
    };
} // namespace dt::df
