#include "base_modal.hpp"
#include <imgui.h>

namespace gui
{
    BaseModal::BaseModal(const std::string &name, int window_flags, const Position position)
        : name_{name}, want_to_open_{false}, is_open_{true}, win_flags_{window_flags}, position_{position}
    {
    }

    void BaseModal::open()
    {
        want_to_open_ = true;
        is_open_ = true;
    }

    void BaseModal::draw()
    {
        if (want_to_open_)
        {
            ImGui::OpenPopup(name_.c_str());
            want_to_open_ = false;
        }
        if (position_ == Position::center)
        {
            // Always center this window when appearing
            const auto &io = ImGui::GetIO();
            const ImVec2 center(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        }
        if (ImGui::BeginPopupModal(name_.c_str(), &is_open_, win_flags_))
        {
            drawContent();
            ImGui::EndPopup();
        }
    }

    const std::string &BaseModal::name() const
    {
        return name_;
    }
} // namespace gui
