#include "ui_node.hpp"
#include <imgui.h>
#include <imnodes.h>
namespace gui::df
{
    UiNode::UiNode(const int vertex_id,
                   const std::string &title,
                   const Slots &inputs,
                   const Slots &outputs)
        : vertex_id_{vertex_id},
          inputs_{inputs},
          outputs_{outputs},
          title_{title}
    {
    }

    void UiNode::draw(const Graph &g)
    {
        imnodes::BeginNode(vertex_id_);
        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(title_.c_str());
        imnodes::EndNodeTitleBar();

        for (auto &slot : inputs_)
        {
            imnodes::BeginInputAttribute(slot.id());
            slot.draw();
            imnodes::EndInputAttribute();
        }

        //drawCustomContent();

        for (auto &slot : outputs_)
        {
            imnodes::BeginOutputAttribute(slot.id());
            slot.draw();
            imnodes::EndOutputAttribute();
        }
        imnodes::EndNode();
    }

    UiNode::Slots &UiNode::inputs()
    {
        return inputs_;
    }
    UiNode::Slots &UiNode::outputs()
    {
        return outputs_;
    }
} // namespace gui::df
