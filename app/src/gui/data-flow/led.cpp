#include "led.hpp"
#include <data-flow/slots/trigger_slot.hpp>
#include <data-flow/slots/floating_slot.hpp>
#include <data-flow/slots/int_slot.hpp>
#include <imgui.h>

using namespace std::chrono_literals;

LED::LED(const dt::df::NodeId id,
         const dt::df::SlotId trigger_id,
         const dt::df::SlotId light_dur_id,
         const dt::df::SlotId r_id,
         const dt::df::SlotId g_id,
         const dt::df::SlotId b_id,
         const dt::df::SlotId a_id)
    : BaseNode(id, kNodeKey, "LED",
               dt::df::Slots{
                   std::make_shared<dt::df::TriggerSlot>(trigger_id, dt::df::SlotType::input),
                   std::make_shared<dt::df::IntSlot>(light_dur_id, dt::df::SlotType::input, "duration (ms)"),
                   std::make_shared<dt::df::FloatingSlot>(r_id, dt::df::SlotType::input, "r"),
                   std::make_shared<dt::df::FloatingSlot>(g_id, dt::df::SlotType::input, "g"),
                   std::make_shared<dt::df::FloatingSlot>(b_id, dt::df::SlotType::input, "b"),
                   std::make_shared<dt::df::FloatingSlot>(a_id, dt::df::SlotType::input, "a")},
               dt::df::Slots{}),
      color_{0.f, 1.f, 0.f, 1.f},
      dur_{50ms}
{
    inputs()[0]->subscribe([this](const dt::df::BaseSlot *) {
        tp_ = std::chrono::system_clock::now();
    });
    inputs()[1]->subscribe([this](const dt::df::BaseSlot *slot) {
        auto int_slot = static_cast<const dt::df::IntSlot *>(slot);
        dur_ = std::chrono::milliseconds(int_slot->valueInt());
    });
    inputs()[2]->subscribe([this](const dt::df::BaseSlot *slot) {
        auto in_slot = static_cast<const dt::df::FloatingSlot *>(slot);
        color_.Value.x = in_slot->value();
    });
    inputs()[3]->subscribe([this](const dt::df::BaseSlot *slot) {
        auto in_slot = static_cast<const dt::df::FloatingSlot *>(slot);
        color_.Value.y = in_slot->value();
    });
    inputs()[4]->subscribe([this](const dt::df::BaseSlot *slot) {
        auto in_slot = static_cast<const dt::df::FloatingSlot *>(slot);
        color_.Value.z = in_slot->value();
    });
    inputs()[5]->subscribe([this](const dt::df::BaseSlot *slot) {
        auto in_slot = static_cast<const dt::df::FloatingSlot *>(slot);
        color_.Value.w = in_slot->value();
    });
}

static dt::df::Slots makeInputs(const nlohmann::json &json)
{
    dt::df::Slots slots;
    try
    {
        const auto &inputs = json.at("inputs");
        for (const auto &in : inputs)
        {
            if (in.at("name") == "trigger")
                slots.emplace_back(std::make_shared<dt::df::TriggerSlot>(in));
            else if (in.at("name") == "duration (ms)")
                slots.emplace_back(std::make_shared<dt::df::IntSlot>(in));
            else
                slots.emplace_back(std::make_shared<dt::df::FloatingSlot>(in));
        }
    }
    catch (...)
    {
    }
    return slots;
}
LED::LED(const nlohmann::json &json)
    : BaseNode(json, makeInputs(json), dt::df::Slots{}),
      color_{colorFrom(json)},
      dur_{50ms}
{
    for (const auto &in_slot : inputs())
    {
        const auto &name = in_slot->name();
        if (name == "trigger")
        {
            in_slot->subscribe([this](const dt::df::BaseSlot *) {
                tp_ = std::chrono::system_clock::now();
            });
        }
        else if (name == "duration (ms)")
        {
            in_slot->subscribe([this](const dt::df::BaseSlot *slot) {
                auto int_slot = static_cast<const dt::df::IntSlot *>(slot);
                dur_ = std::chrono::milliseconds(int_slot->valueInt());
            });
        }
        else if (name == "r")
        {
            in_slot->subscribe([this](const dt::df::BaseSlot *slot) {
                auto in_slot = static_cast<const dt::df::FloatingSlot *>(slot);
                color_.Value.x = in_slot->value();
            });
        }
        else if (name == "g")
        {
            in_slot->subscribe([this](const dt::df::BaseSlot *slot) {
                auto in_slot = static_cast<const dt::df::FloatingSlot *>(slot);
                color_.Value.y = in_slot->value();
            });
        }
        else if (name == "b")
        {
            in_slot->subscribe([this](const dt::df::BaseSlot *slot) {
                auto in_slot = static_cast<const dt::df::FloatingSlot *>(slot);
                color_.Value.z = in_slot->value();
            });
        }
        else if (name == "a")
        {
            in_slot->subscribe([this](const dt::df::BaseSlot *slot) {
                auto in_slot = static_cast<const dt::df::FloatingSlot *>(slot);
                color_.Value.w = in_slot->value();
            });
        }
    }
}

void LED::to_json(nlohmann::json &j) const
{
    BaseNode::to_json(j);
    j["color"] = {
        {"r", color_.Value.x},
        {"g", color_.Value.y},
        {"b", color_.Value.z},
        {"a", color_.Value.w},
    };
}

void LED::renderCustomContent()
{
    const auto expired = std::chrono::system_clock::now() - tp_;
    const ImVec2 p = ImGui::GetCursorScreenPos();
    const float size = 36.f;
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    const auto pos = ImVec2{p.x + size * 0.5f, p.y + size * 0.5f};
    if (std::chrono::duration_cast<std::chrono::milliseconds>(expired) > dur_)
    {
        draw_list->AddCircle(pos, size * 0.5f, color_);
    }
    else
    {
        draw_list->AddCircleFilled(pos, size * 0.5f, color_);
    }
    ImGui::Dummy({size, size});
}

LED::~LED()
{
}

ImColor LED::colorFrom(const nlohmann::json &j)
{
    ImColor color;
    try
    {
        const auto &color_j = j.at("color");
        color = ImColor(color_j.at("r").get<float>(),
                        color_j.at("g").get<float>(),
                        color_j.at("b").get<float>(),
                        color_j.at("a").get<float>());
    }
    catch (...)
    {
    }
    return color;
}
