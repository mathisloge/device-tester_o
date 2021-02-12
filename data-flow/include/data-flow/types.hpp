#pragma once
#include <string>
#include <memory>
#include <atomic>

#include "nodes/base_node.hpp"
namespace dt::df
{
    using EdgeId = int;
    using NodePtr = std::shared_ptr<BaseNode>;
    using NodeIdGenerator = std::atomic_int;
    using SlotIdGenerator = std::atomic_int;
    using NodeFactory = std::function<NodePtr(NodeIdGenerator &, SlotIdGenerator &)>;
} // namespace dt::df
