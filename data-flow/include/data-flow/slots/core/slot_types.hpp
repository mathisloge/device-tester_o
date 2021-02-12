#pragma once
#include <string>
namespace dt::df
{
    using SlotId = int;
    using SlotName = std::string;
    enum class SlotType
    {
        input,
        output
    };

    enum class SlotFieldVisibility
    {
        never,
        without_connection,
        always
    };
} // namespace dt::df
