#pragma once
#include <data-flow/nodes/base_node.hpp>
#include <chrono>

class LED final : public dt::df::BaseNode
{
public:
    static constexpr const char *kNodeKey = "led-node";

public:
    explicit LED(const dt::df::NodeId id,
                 const dt::df::SlotId trigger_id);
    const dt::df::NodeKey &key() const override;
    ~LED();

private:
    void renderCustomContent() override;

private:
    const std::string &node_key_;
    std::chrono::system_clock::time_point tp_;
};
