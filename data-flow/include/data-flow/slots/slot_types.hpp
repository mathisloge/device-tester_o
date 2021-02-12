#pragma once
namespace dt::df
{
    using SlotId = int;

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
