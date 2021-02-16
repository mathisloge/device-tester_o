#pragma once
#include <string>
#include <string_view>
namespace dt::df
{
    using SlotId = int;
    using SlotName = std::string;
    using SlotKey = std::string_view;
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
