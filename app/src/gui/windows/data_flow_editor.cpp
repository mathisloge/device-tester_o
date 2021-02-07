#include "data_flow_editor.hpp"
#include <spdlog/spdlog.h>
#include <imgui.h>
#include "../data_flow/node.hpp"
namespace gui
{

    DataFlowEditor::DataFlowEditor(const std::string &win_name, protocol::ProtocolLoader &proto_loader)
        : BaseWindow(win_name),
          proto_loader_{proto_loader}
    {
        flags_ = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
        refreshProtocols();

        data_flow_graph_.registerNodeFactory("test", []() {
            auto node = std::make_shared<df::Node>(
                "Test",
                df::Node::InSlots{df::InputSlot{"Int", "int"}, df::InputSlot{"string", "string"}},
                df::Node::OutSlots{df::OutputSlot{"Int", "int"}, df::OutputSlot{"string", "string"}});
            return node;
        });
        data_flow_graph_.registerNodeFactory("test2", []() {
            auto node = std::make_shared<df::Node>(
                "Test2",
                df::Node::InSlots{df::InputSlot{"Int", "int"}, df::InputSlot{"string", "string"}},
                df::Node::OutSlots{df::OutputSlot{"Int", "int"}, df::OutputSlot{"string", "string"}});
            return node;
        });
    }

    void DataFlowEditor::drawContent()
    {
        constexpr int kSidebarWidth = 250;
        { // sidebar
            ImGui::BeginChild("sidbar_data_flow", ImVec2{kSidebarWidth, 0}, true);
            if (ImGui::Button("Refresh protocols", ImVec2{kSidebarWidth, 0}))
                refreshProtocols();

            if (ImGui::Button("ADD TEST NDOE"))
            {
                data_flow_graph_.addNode("test");
            }
            if (ImGui::Button("ADD TEST NDOE2"))
            {
                data_flow_graph_.addNode("test2");
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

            data_flow_graph_.drawNodes();
            data_flow_graph_.drawLinks();

            imnodes::EndNodeEditor();
            data_flow_graph_.addPendingConnections();
            data_flow_graph_.deletePendingConnections();

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
