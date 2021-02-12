#pragma once
#include <string>
#include <memory>

#include "nodes/core/base_node.hpp"
#include "id_generator.hpp"
namespace dt::df
{
    using EdgeId = int;
    using NodePtr = std::shared_ptr<BaseNode>;
    using NodeIdGenerator = IdGenerator;
    using SlotIdGenerator = IdGenerator;
    using NodeFactory = std::function<NodePtr(NodeIdGenerator &, SlotIdGenerator &)>;
} // namespace dt::df
