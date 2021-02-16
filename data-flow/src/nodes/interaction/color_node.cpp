#include <imgui.h>
#include "data-flow/nodes/interaction/color_node.hpp"
#include "data-flow/slots/int_slot.hpp"

namespace dt::df
{
    class ColorNode::Impl final
    {
    public:
        Impl() : color_vals_{0.f, 0.f, 0.f, 0.f},
                 color_vals_prev_{0.f, 0.f, 0.f, 0.f} {}
        bool checkColorIndex(int idx) const
        {
            return color_vals_prev_[idx] != color_vals_[idx];
        }
        int color(int idx) const
        {
            return static_cast<int>(color_vals_[idx] * 255);
        }
        void copyColor()
        {
            for (int i = 0; i < 4; i++)
                color_vals_prev_[i] = color_vals_[i];
        }

    private:
        float color_vals_[4];
        float color_vals_prev_[4];
        friend ColorNode;

        std::shared_ptr<IntSlot> r_slot_;
        std::shared_ptr<IntSlot> g_slot_;
        std::shared_ptr<IntSlot> b_slot_;
        std::shared_ptr<IntSlot> a_slot_;
    };

    static Slots makeOutputSlotsFromJson(const nlohmann::json &json)
    {
        Slots output_slots;
        try
        {
            auto &ojson = json.at("outputs");
            for (const auto &j_output : ojson)
            {
                output_slots.push_back(std::make_shared<IntSlot>(j_output));
            }
        }
        catch (...)
        {
        }
        return output_slots;
    }

    ColorNode::ColorNode(const NodeId id,
                         const SlotId output_r,
                         const SlotId output_g,
                         const SlotId output_b,
                         const SlotId output_a)
        : BaseNode{id, kNodeKey, "Color",
                   Slots{},
                   Slots{
                       std::make_shared<IntSlot>(IntSlot::kKey, output_r, SlotType::output, "r", 0, SlotFieldVisibility::never),
                       std::make_shared<IntSlot>(IntSlot::kKey, output_g, SlotType::output, "g", 1, SlotFieldVisibility::never),
                       std::make_shared<IntSlot>(IntSlot::kKey, output_b, SlotType::output, "b", 2, SlotFieldVisibility::never),
                       std::make_shared<IntSlot>(IntSlot::kKey, output_a, SlotType::output, "a", 3, SlotFieldVisibility::never)}},
          impl_{new Impl{}}
    {
        impl_->r_slot_ = std::dynamic_pointer_cast<IntSlot>(outputByLocalId(0));
        impl_->g_slot_ = std::dynamic_pointer_cast<IntSlot>(outputByLocalId(1));
        impl_->b_slot_ = std::dynamic_pointer_cast<IntSlot>(outputByLocalId(2));
        impl_->a_slot_ = std::dynamic_pointer_cast<IntSlot>(outputByLocalId(3));
    }

    ColorNode::ColorNode(const nlohmann::json &json)
        : BaseNode{json, Slots{}, makeOutputSlotsFromJson(json)},
          impl_{new Impl{}}
    {
        impl_->r_slot_ = std::dynamic_pointer_cast<IntSlot>(outputByLocalId(0));
        impl_->g_slot_ = std::dynamic_pointer_cast<IntSlot>(outputByLocalId(1));
        impl_->b_slot_ = std::dynamic_pointer_cast<IntSlot>(outputByLocalId(2));
        impl_->a_slot_ = std::dynamic_pointer_cast<IntSlot>(outputByLocalId(3));
    }

    void ColorNode::renderCustomContent()
    {
        ImGui::PushID(this);
        ImGui::SetNextItemWidth(150.f);
        if (ImGui::ColorEdit4("##colorpicker", impl_->color_vals_))
        {
            if (impl_->checkColorIndex(0))
                impl_->r_slot_->setValue(impl_->color(0));
            if (impl_->checkColorIndex(1))
                impl_->g_slot_->setValue(impl_->color(1));
            if (impl_->checkColorIndex(2))
                impl_->b_slot_->setValue(impl_->color(2));
            if (impl_->checkColorIndex(3))
                impl_->a_slot_->setValue(impl_->color(3));

            impl_->copyColor();
        }
        ImGui::PopID();
    }

    ColorNode::~ColorNode()
    {
        delete impl_;
    }
} // namespace dt::df
