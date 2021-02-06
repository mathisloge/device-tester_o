#include "base_window.hpp"
#include <imgui.h>
namespace gui
{
    BaseWindow::BaseWindow(const std::string &name)
        : name_{name}, is_open_{true}, flags_{0}
    {
    }

    const std::string &BaseWindow::name() const
    {
        return name_;
    }

    void BaseWindow::draw()
    {
        if (!is_open_)
            return;
        if (ImGui::Begin(name_.c_str(), &is_open_, flags_))
        {
            drawContent();
        }
        ImGui::End();
    }

    bool &BaseWindow::openState()
    {
        return is_open_;
    }

    bool BaseWindow::isOpen() const
    {
        return is_open_;
    }
} // namespace gui
