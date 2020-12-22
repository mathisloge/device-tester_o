#include "connection_win.hpp"
#include <imgui.h>
namespace gui
{
    ConnectionWin::ConnectionWin(const std::string &name, DeviceConnection &device_connection)
        : BaseWindow(name)
    {
        device_connection.connectData([this](std::span<uint8_t> data) { raw_text_.addData(data); });
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
            if (ImGui::BeginTabItem("Raw output"))
            {
                raw_text_.draw();
                ImGui::Separator();
                drawConnectionRawInput();
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
