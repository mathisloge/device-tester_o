#include "connection_win.hpp"
#include <imgui.h>
#include "../imgui_commons.hpp"
namespace gui
{
    ConnectionWin::ConnectionWin(const std::string &name, DeviceConnection &device_connection)
        : BaseWindow(name)
    {
        device_connection.connectData([this](std::span<uint8_t> data) {
            raw_text_.addData(data);
            //raw_mem_viewer_.addData(data);
            data_throughput_.addData(data);
        });
    }

    void ConnectionWin::drawContent()
    {
        ImGui::PushID(this);
        if (!error_str_.empty())
            ImGui::TextColored(kErrorColor, "Error: %s", error_str_.c_str());

        if (ImGui::BeginTabBar("ConnectionTabBar"))
        {
            if (ImGui::BeginTabItem("Overview"))
            {
                drawConnectionOverview();
                data_throughput_.drawPlot();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Raw output"))
            {
                raw_text_.draw();
                ImGui::Separator();
                drawConnectionRawInput();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Hex viewer"))
            {
                //raw_mem_viewer_.draw();
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
