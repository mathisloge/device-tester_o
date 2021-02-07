#include "node.hpp"
#include <spdlog/spdlog.h>
#include <imnodes.h>
namespace gui::df
{
    Node::Node(const std::string &title,
               InSlots &&input_slots,
               OutSlots &&output_slots)
        : id_{node_id_counter++},
          title_{title},
          is_selected_{false}
    {
        std::transform(input_slots.begin(), input_slots.end(), std::inserter(input_slots_, input_slots_.end()),
                       [](const InputSlot &s) { return std::make_pair(s.id(), std::move(s)); });
        std::transform(output_slots.begin(), output_slots.end(), std::inserter(output_slots_, output_slots_.end()),
                       [](const OutputSlot &s) { return std::make_pair(s.id(), std::move(s)); });
    }

    int Node::id() const
    {
        return id_;
    }

    void Node::drawNode()
    {
        imnodes::BeginNode(id_);
        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(title_.c_str());
        imnodes::EndNodeTitleBar();

        for (auto &slot : input_slots_)
        {
            slot.second.drawSlot();
        }

        drawCustomContent();

        for (auto &slot : output_slots_)
        {
            slot.second.drawSlot();
        }
        imnodes::EndNode();
    }

    const Node::InSlotsContainer &Node::input_slots() const
    {
        return input_slots_;
    }
    const Node::OutSlotsContainer &Node::output_slots() const
    {
        return output_slots_;
    }

    void Node::drawCustomContent()
    {
    }
} // namespace gui::df
