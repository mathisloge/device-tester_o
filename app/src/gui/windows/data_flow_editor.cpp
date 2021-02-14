#include "data_flow_editor.hpp"
#include <spdlog/spdlog.h>
#include <imgui.h>
#include <data-flow/nodes/utils/timer_node.hpp>
#include <data-flow/nodes/core/base_node.hpp>
#include <data-flow/slots/int_slot.hpp>
#include "../data-flow/led.hpp"
namespace gui
{

    DataFlowEditor::DataFlowEditor(const std::string &win_name, protocol::ProtocolLoader &proto_loader)
        : BaseWindow(win_name),
          proto_loader_{proto_loader}
    {
        flags_ = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
        refreshProtocols();

        df_editor_.graph().registerBuildinNodes();

        df_editor_.graph().registerNode(
            "int-node",
            [](dt::df::NodeIdGenerator &node_id, dt::df::SlotIdGenerator &slot_id) {
                return std::make_shared<dt::df::BaseNode>(node_id(), "int-node", "INT",
                                                          dt::df::Slots{},
                                                          dt::df::Slots{std::make_shared<dt::df::IntSlot>(slot_id(), dt::df::SlotType::output, "int", dt::df::SlotFieldVisibility::always)});
            },
            [](const nlohmann::json &j) {
                return std::make_shared<dt::df::BaseNode>(j,
                                                          dt::df::Slots{},
                                                          dt::df::Slots{std::make_shared<dt::df::IntSlot>(j["outputs"][0])});
            });

        df_editor_.graph().registerNode(
            LED::kNodeKey,
            [](dt::df::NodeIdGenerator &node_id, dt::df::SlotIdGenerator &slot_id) {
                return std::make_shared<LED>(node_id(), slot_id(), slot_id(), slot_id(), slot_id(), slot_id(), slot_id());
            },
            [](const nlohmann::json &j) {
                return std::make_shared<LED>(j);
            });

        df_editor_.graph().clearAndLoad("test.json");
    }

    void DataFlowEditor::drawContent()
    {
        constexpr int kSidebarWidth = 250;
        { // sidebar
            ImGui::BeginChild("sidbar_data_flow", ImVec2{kSidebarWidth, 0}, true);
            if (ImGui::Button("Refresh protocols", ImVec2{kSidebarWidth, 0}))
                refreshProtocols();

            if (ImGui::Button("ADD TIMER NODE"))
            {
                df_editor_.graph().addNode(dt::df::TimerNode::kNodeKey);
            }
            if (ImGui::Button("ADD COLOR NODE"))
            {
                df_editor_.graph().addNode("color-node");
            }
            if (ImGui::Button("ADD DIV NODE"))
                df_editor_.graph().addNode("Division-op-node");

            if (ImGui::Button("ADD INT NODE"))
            {
                df_editor_.graph().addNode("int-node");
            }

            if (ImGui::Button("ADD LED NODE"))
            {
                df_editor_.graph().addNode(LED::kNodeKey);
            }

            if (ImGui::Button("SAVE"))
            {
                df_editor_.graph().save("test.json");
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

            df_editor_.render();

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
