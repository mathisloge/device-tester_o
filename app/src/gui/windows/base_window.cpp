#include "base_window.hpp"
#include <imgui.h>
namespace gui
{
    constexpr BaseWindow::BaseWindow(const std::string &name)
        : name_{name}, is_open_{true}
    {
    }

    void BaseWindow::draw()
    {
        if (!is_open_)
            return;
        if (ImGui::Begin(name_.c_str(), &is_open_))
        {
            drawContent();
        }
        ImGui::End();
    }

    void BaseWindow::setOpenState(bool is_open)
    {
        is_open_ = is_open;
    }

    bool BaseWindow::isOpen() const
    {
        return is_open_;
    }
} // namespace gui
