#pragma once
#include <nlohmann/json.hpp>
#include "data-flow/nodes/core/base_node.hpp"

namespace dt::df
{
    //! these will directly be implemented in base_node.cpp
    void to_json(nlohmann::json &j, const BaseNode &p);
} // namespace dt::df
