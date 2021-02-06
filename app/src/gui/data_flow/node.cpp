#include "node.hpp"
#include <ImNodes.h>
#include <ImNodesEz.h>
#include <spdlog/spdlog.h>
namespace gui::df
{
    Node::Node(const std::string &title,
               const Slots &input_slots,
               const Slots &output_slots)
        : title_{title},
          input_slots_{input_slots},
          output_slots_{output_slots},
          is_selected_{false}
    {
    }

    void Node::deleteConnection(const Connection &connection)
    {
        auto it = std::find_if(connections_.begin(), connections_.end(), [&connection](const auto &con) { return con == connection; });
        if (it != connections_.end())
            connections_.erase(it);
    }

    void Node::drawNode()
    {
        if (!ImNodes::Ez::BeginNode(this, title_.c_str(), &position_, &is_selected_))
        {
            ImNodes::Ez::EndNode();
            return;
        }

        ImNodes::Ez::InputSlots(input_slots_.data(), input_slots_.size());
        drawCustomContent();
        ImNodes::Ez::OutputSlots(output_slots_.data(), output_slots_.size());

        Connection new_connection;
        if (ImNodes::GetNewConnection(reinterpret_cast<void **>(&new_connection.input_node),
                                      &new_connection.input_slot,
                                      reinterpret_cast<void **>(&new_connection.output_node),
                                      &new_connection.output_slot))
        {
        }

        for (auto &connection : connections_)
        {
            if (connection.output_node != this)
                continue;

            if (!ImNodes::Connection(&connection.input_node, connection.input_slot, this, connection.output_slot))
            {
                connection.input_node->deleteConnection(connection);
                connection.output_node->deleteConnection(connection);
            }
        }

        ImNodes::Ez::EndNode();
    }

    void Node::drawCustomContent()
    {
    }
} // namespace gui::df
