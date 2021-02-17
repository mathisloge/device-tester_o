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
            "utils/Int Node",
            [](dt::df::NodeIdGenerator &node_id, dt::df::SlotIdGenerator &slot_id) {
                return std::make_shared<dt::df::BaseNode>(node_id(), "int-node", "INT",
                                                          dt::df::Slots{},
                                                          dt::df::Slots{std::make_shared<dt::df::IntSlot>(dt::df::IntSlot::kKey, slot_id(), dt::df::SlotType::output, "int", 0, dt::df::SlotFieldVisibility::always)});
            },
            [](const nlohmann::json &j) {
                return std::make_shared<dt::df::BaseNode>(j,
                                                          dt::df::Slots{},
                                                          dt::df::Slots{std::make_shared<dt::df::IntSlot>(j["outputs"][0])});
            });

        df_editor_.graph().registerNode(
            LED::kNodeKey,
            "interaction/LED",
            [](dt::df::NodeIdGenerator &node_id, dt::df::SlotIdGenerator &slot_id) {
                return std::make_shared<LED>(node_id(), slot_id(), slot_id(), slot_id(), slot_id(), slot_id(), slot_id());
            },
            [](const nlohmann::json &j) {
                return std::make_shared<LED>(j);
            });
    }

    void DataFlowEditor::drawContent()
    {
        constexpr int kSidebarWidth = 250;
        { // sidebar
            ImGui::BeginChild("sidbar_data_flow", ImVec2{kSidebarWidth, 0}, true);
            if (ImGui::Button("Refresh protocols", ImVec2{kSidebarWidth, 0}))
                refreshProtocols();
            if (ImGui::Button("Clear graph", ImVec2{kSidebarWidth, 0}))
                df_editor_.graph().clear();

            if (ImGui::Button("Save current", ImVec2{kSidebarWidth, 0}))
                df_editor_.graph().save("test.json");

            if (ImGui::Button("Load", ImVec2{kSidebarWidth, 0}))
                df_editor_.graph().clearAndLoad("test.json");

            {
                bool need_next_draw = true;
                int level_draw = -1;
                df_editor_.renderNodeDisplayTree([&need_next_draw, &level_draw](int prev_level, int level, bool is_leaf, const std::string &node_key, const std::string &node_name) {
                    if (!need_next_draw && level_draw < level)
                        return;
                    if (!is_leaf && prev_level < level)
                    {
                        need_next_draw = ImGui::TreeNode(node_name.c_str());
                        level_draw = level;
                    }
                    else if (!is_leaf && prev_level == level)
                    {
                        if (need_next_draw)
                            ImGui::TreePop();
                        need_next_draw = true;
                        level_draw = 0;
                    }
                    else if (is_leaf && prev_level < level)
                    {
                        ImGui::SmallButton(node_name.c_str());
                        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                        {
                            ImGui::SetDragDropPayload(kDndTarget, node_key.c_str(), node_key.size());
                            ImGui::Text(node_name.c_str());
                            ImGui::EndDragDropSource();
                        }
                    }
                });
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
