#pragma once
#include <boost/noncopyable.hpp>
#include <string>
namespace gui::df
{
    class Node;

    struct Connection final
    {
        Node *input_node = nullptr;
        const char* input_slot;

        Node *output_node = nullptr;
        const char*  output_slot;

        bool operator==(const Connection &other) const;
        bool operator!=(const Connection &other) const;
    };
} // namespace gui::df
