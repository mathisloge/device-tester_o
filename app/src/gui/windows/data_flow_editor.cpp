#include "data_flow_editor.hpp"
#include <spdlog/spdlog.h>
#include <imgui.h>
#include <ImNodesEz.h>
namespace gui
{

    DataFlowEditor::DataFlowEditor(const std::string &win_name)
        : BaseWindow(win_name)
    {
        flags_ = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
    }

    void DataFlowEditor::drawContent()
    {
        { // sidebar
            ImGui::BeginChild("sidbar_data_flow", ImVec2(150, 0), true);
            if (ImGui::TreeNode("Protocols"))
            {
                if (ImGui::TreeNode("Nmea"))
                {
                    ImGui::Button("Messagex");

                    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                    {
                        ImGui::SetDragDropPayload("DND_DATAFLOW", this, sizeof(this));
                        ImGui::Text("Messagex");
                        ImGui::EndDragDropSource();
                    }
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Ublox"))
                {
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
            ImGui::EndChild();
        }
        ImGui::SameLine();
        // content window
        {
            ImGui::BeginChild("data flow canvas view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
            ImNodes::BeginCanvas(&canvas_);
            handleDnD();

            struct Node
            {
                ImVec2 pos{};
                bool selected{};
                ImNodes::Ez::SlotInfo inputs[1];
                ImNodes::Ez::SlotInfo outputs[1];
            };

            static Node nodes[3] = {
                {{50, 100}, false, {{"In", 1}}, {{"Out", 1}}},
                {{250, 50}, false, {{"In", 1}}, {{"Out", 1}}},
                {{250, 100}, false, {{"In", 1}}, {{"Out", 1}}},
            };

            for (Node &node : nodes)
            {
                if (ImNodes::Ez::BeginNode(&node, "Node Title", &node.pos, &node.selected))
                {
                    ImNodes::Ez::InputSlots(node.inputs, 1);
                    ImNodes::Ez::OutputSlots(node.outputs, 1);
                    ImNodes::Ez::EndNode();
                }
            }

            ImNodes::Connection(&nodes[1], "In", &nodes[0], "Out");
            ImNodes::Connection(&nodes[2], "In", &nodes[0], "Out");

            ImNodes::EndCanvas();
            ImGui::EndChild();
        }
    }

    void DataFlowEditor::handleDnD()
    {
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("DND_DATAFLOW"))
            {
                SPDLOG_DEBUG("got payload");
            }
            ImGui::EndDragDropTarget();
        }
    }
} // namespace gui
