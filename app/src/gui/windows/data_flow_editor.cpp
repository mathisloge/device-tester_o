#include "data_flow_editor.hpp"
#include <spdlog/spdlog.h>
#include <imgui.h>
#include <ImNodesEz.h>
namespace gui
{

    DataFlowEditor::DataFlowEditor(const std::string &win_name, protocol::ProtocolLoader &proto_loader)
        : BaseWindow(win_name),
          proto_loader_{proto_loader}
    {
        flags_ = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
        refreshProtocols();
    }

    void DataFlowEditor::drawContent()
    {
        constexpr int kSidebarWidth = 250;
        { // sidebar
            ImGui::BeginChild("sidbar_data_flow", ImVec2{kSidebarWidth, 0}, true);
            if (ImGui::Button("Refresh protocols", ImVec2{kSidebarWidth, 0}))
                refreshProtocols();

            if (ImGui::TreeNode("Protocols"))
            {
                drawProtocols();
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

    void DataFlowEditor::drawProtocols()
    {
        for (const auto &proto_name : protocol_names_)
        {
            if (ImGui::TreeNode(proto_name.c_str()))
            {
                ImGui::Button("Protocol instance");
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                {
                    ImGui::SetDragDropPayload(kDndTarget, this, sizeof(this));
                    ImGui::Text(proto_name.c_str());
                    ImGui::EndDragDropSource();
                }
                ImGui::Indent();
                for (int i = 0; i < 10; i++)
                {
                    const auto btn_name = fmt::format("Message {}", i);
                    ImGui::Button(btn_name.c_str());
                    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                    {
                        ImGui::SetDragDropPayload(kDndTarget, this, sizeof(this));
                        ImGui::Text(btn_name.c_str());
                        ImGui::EndDragDropSource();
                    }
                }

                ImGui::Unindent();

                ImGui::TreePop();
            }
        }
    }

    void DataFlowEditor::handleDnD()
    {
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload(kDndTarget))
            {
                SPDLOG_DEBUG("got payload");
            }
            ImGui::EndDragDropTarget();
        }
    }

    void DataFlowEditor::refreshProtocols()
    {
        protocol_names_ = proto_loader_.findAllProtocols();
    }
} // namespace gui
