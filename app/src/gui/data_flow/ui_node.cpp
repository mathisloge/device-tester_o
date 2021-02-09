#include "ui_node.hpp"
#include <imgui.h>
#include <imnodes.h>
namespace gui::df
{
    UiNode::UiNode(const int vertex_desc,
                   const std::string &title,
                   const Slots &inputs,
                   const Slots &outputs)
        : vertex_desc_{vertex_desc},
          inputs_{inputs},
          outputs_{outputs},
          title_{title}
    {
    }

    void UiNode::draw(const Graph &g)
    {
        imnodes::BeginNode(vertex_desc_);
        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(title_.c_str());
        imnodes::EndNodeTitleBar();

        for (auto &slot : inputs_)
        {
            imnodes::BeginInputAttribute(slot.vertexDesc());
            slot.draw();
            imnodes::EndInputAttribute();
        }

        //drawCustomContent();

        for (auto &slot : outputs_)
        {
            imnodes::BeginOutputAttribute(slot.vertexDesc());
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
