#include "led.hpp"
#include <data-flow/slots/trigger_slot.hpp>
#include <imgui.h>

using namespace std::chrono_literals;

LED::LED(const dt::df::NodeId id,
         const dt::df::SlotId trigger_id)
    : BaseNode(id, "LED", dt::df::Slots{std::make_shared<dt::df::TriggerSlot>(trigger_id, dt::df::SlotType::input)}, dt::df::Slots{}),
      node_key_{kNodeKey}
{
    this->inputs()[0]->subscribe([this]() {
        tp_ = std::chrono::system_clock::now();
    });
}

const dt::df::NodeKey &LED::key() const
{
    return node_key_;
}

void LED::renderCustomContent()
{
    const auto expired = std::chrono::system_clock::now() - tp_;
    const ImU32 col = ImColor(0.f, 1.f, 0.f, 1.f);
    const ImVec2 p = ImGui::GetCursorScreenPos();
    const float size = 36.f;
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    const auto pos = ImVec2{p.x + size * 0.5f, p.y + size * 0.5f};
    if (std::chrono::duration_cast<std::chrono::milliseconds>(expired) > 50ms)
    {
        draw_list->AddCircle(pos, size * 0.5f, col);
    }
    else
    {
        draw_list->AddCircleFilled(pos, size * 0.5f, col);
    }
    ImGui::Dummy({size, size});
}

LED::~LED()
{
}
