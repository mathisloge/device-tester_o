#include "inputs.hpp"
#include <imgui_stdlib.h>
#include <imgui_internal.h>

namespace gui
{
    static int InputTextCallback(ImGuiInputTextCallbackData *data)
    {
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
        {
            // Resize string callback
            std::string *str = (std::string *)data->UserData;
            IM_ASSERT(data->Buf == str->c_str());
            str->resize(data->BufTextLen);
            data->Buf = (char *)str->c_str();
        }
        return 0;
    }

    bool SimpleInputText(const char *label, std::string *str, ImGuiInputTextFlags flags)
    {
        return ImGui::InputText(label, str, flags, InputTextCallback, (void *)str);
    }

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
