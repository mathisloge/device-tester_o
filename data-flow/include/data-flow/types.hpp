#pragma once
#include <string>
#include <memory>
#include <nlohmann/json.hpp>

#include "nodes/core/base_node.hpp"
#include "id_generator.hpp"
namespace dt::df
{
    using EdgeId = int;
    using NodePtr = std::shared_ptr<BaseNode>;
    using NodeIdGenerator = IdGenerator;
    using SlotIdGenerator = IdGenerator;
    using NodeFactory = std::function<NodePtr(NodeIdGenerator &, SlotIdGenerator &)>;
    using NodeDeserializationFactory = std::function<NodePtr(const nlohmann::json &)>;
    using NodeDisplayDrawFnc = std::function<void(int prev_level, int level, bool is_leaf, const std::string &node_key, const std::string &node_name)>;

} // namespace dt::df
