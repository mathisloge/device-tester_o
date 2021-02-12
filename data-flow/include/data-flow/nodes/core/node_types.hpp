#pragma once
#include <string>
#include <vector>
#include <memory>
#include "data-flow/slots/core/base_slot.hpp"
namespace dt::df
{
    using NodeId = int;
    using NodeKey = std::string;
    using SlotPtr = std::shared_ptr<BaseSlot>;
    using Slots = std::vector<SlotPtr>;
} // namespace dt::df
