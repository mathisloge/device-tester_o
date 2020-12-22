#include "connection_win.hpp"
#include <imgui.h>
namespace gui
{
    ConnectionWin::ConnectionWin(const std::string &name)
        : BaseWindow(name)
    {
    }
    
    void ConnectionWin::drawContent()
    {
        ImGui::PushID(this);
        if (ImGui::BeginTabBar("ConnectionTabBar"))
        {
            if (ImGui::BeginTabItem("Overview"))
            {
                drawConnectionOverview();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Details"))
            {
                drawConnectionDetails();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Settings"))
            {
                drawConnectionSettings();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::PopID();
    }
} // namespace gui
