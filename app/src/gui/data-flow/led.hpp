#pragma once
#include <data-flow/nodes/core/base_node.hpp>
#include <chrono>
#include <imgui.h>
class LED final : public dt::df::BaseNode
{
public:
    static constexpr const char *kNodeKey = "led-node";

public:
    LED(const dt::df::NodeId id,
        const dt::df::SlotId trigger_id,
        const dt::df::SlotId light_dur_id,
        const dt::df::SlotId r_id,
        const dt::df::SlotId g_id,
        const dt::df::SlotId b_id,
        const dt::df::SlotId a_id);
    LED(const nlohmann::json &json);
    void to_json(nlohmann::json &j) const override;
    ~LED();

private:
    void initInputs();
    void renderCustomContent() override;
    static ImColor colorFrom(const nlohmann::json &j);

private:
    std::chrono::system_clock::time_point tp_;
    ImColor color_;
    std::chrono::milliseconds dur_;
};
