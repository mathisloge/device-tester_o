#include "data_flow_editor.hpp"
#include <spdlog/spdlog.h>
#include <imgui.h>
#include <data-flow/nodes/timer_node.hpp>
namespace gui
{

    DataFlowEditor::DataFlowEditor(const std::string &win_name, protocol::ProtocolLoader &proto_loader)
        : BaseWindow(win_name),
          proto_loader_{proto_loader}
    {
        flags_ = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
        refreshProtocols();

        data_flow_graph_.registerBuildinNodes();
    }

    void DataFlowEditor::drawContent()
    {
        constexpr int kSidebarWidth = 250;
        { // sidebar
            ImGui::BeginChild("sidbar_data_flow", ImVec2{kSidebarWidth, 0}, true);
            if (ImGui::Button("Refresh protocols", ImVec2{kSidebarWidth, 0}))
                refreshProtocols();

            if (ImGui::Button("ADD TIMER NDOE"))
            {
                data_flow_graph_.addNode(dt::df::TimerNode::kNodeKey);
            }

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
            ImGui::BeginChild("data flow canvas view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
            imnodes::BeginNodeEditor();
            handleDnD();

            data_flow_graph_.render();

            imnodes::EndNodeEditor();
            { // add pending connections
                int started_at_attribute_id;
                int ended_at_attribute_id;
                if (imnodes::IsLinkCreated(&started_at_attribute_id, &ended_at_attribute_id))
                {
                    data_flow_graph_.addEdge(started_at_attribute_id, ended_at_attribute_id);
                }
            }
            { // delete connection
                int link_id;
                if (imnodes::IsLinkDestroyed(&link_id))
                {
                    data_flow_graph_.removeEdge(link_id);
                }
            }

            {
                const int num_selected = imnodes::NumSelectedNodes();
                if (num_selected > 0 && ImGui::IsKeyReleased(ImGuiKey_Delete))
                {
                    static std::vector<int> selected_nodes;
                    selected_nodes.resize(static_cast<size_t>(num_selected));
                    imnodes::GetSelectedNodes(selected_nodes.data());
                    for (const int node_id : selected_nodes)
                    {
                        data_flow_graph_.removeNode(node_id);
                    }
                }
            }

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
