#pragma once
#include <string>
#include <imgui.h>
namespace gui
{
    bool SimpleInputText(const char *label, std::string *str, ImGuiInputTextFlags flags = 0);
    bool Button(const char *text, bool disabled = true);
} // namespace gui
