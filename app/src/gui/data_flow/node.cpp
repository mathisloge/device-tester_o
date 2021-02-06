#include "node.hpp"
#include <spdlog/spdlog.h>
#include <imnodes.h>
namespace gui::df
{
    Node::Node(const std::string &title,
               const Slots &input_slots,
               const Slots &output_slots)
        : id_{node_id_counter++},
          title_{title},
          input_slots_{input_slots},
          output_slots_{output_slots},
          is_selected_{false}
    {
    }

    void Node::drawNode()
    {
        imnodes::BeginNode(id_);
        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(title_.c_str());
        imnodes::EndNodeTitleBar();

        for (auto &slot : input_slots_)
        {
            slot.drawSlot();
        }
        
        drawCustomContent();

        for (auto &slot : output_slots_)
        {
            slot.drawSlot();
        }
        imnodes::EndNode();
    }

    void Node::drawCustomContent()
    {
    }
} // namespace gui::df
