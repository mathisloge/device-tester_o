#include "imgui_commons.hpp"
#include <imgui.h>
#include <imgui_internal.h>
namespace gui
{
    bool Button(const char *text, bool disabled)
    {
        if (disabled)
        {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        }
        const auto clicked = ImGui::Button(text);
        if (disabled)
        {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar();
        }
        return clicked;
    }
} // namespace gui
