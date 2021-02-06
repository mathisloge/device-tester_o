#include "connection.hpp"
namespace gui::df
{
    bool Connection::operator==(const Connection &other) const
    {
        return input_node == other.input_node &&
               input_slot == other.input_slot &&
               output_node == other.output_node &&
               output_slot == other.output_slot;
    }

    bool Connection::operator!=(const Connection &other) const
    {
        return !operator==(other);
    }
} // namespace gui::df
