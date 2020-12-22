#include "raw_text_output.hpp"
#include <imgui.h>
namespace gui
{
    RawTextOutput::RawTextOutput()
        : auto_scroll_{true}
    {
    }

    void RawTextOutput::draw()
    {
        if (ImGui::Button("Clear"))
        {
            raw_output_ = "";
        }
        ImGui::SameLine();
        ImGui::Checkbox("Autoscroll", &auto_scroll_);
        const float window_height = ImGui::GetWindowHeight();
        static constexpr float kOffsetSize = 150.f;
        if (ImGui::BeginChild("##raw_output_content", ImVec2{-1.f, window_height <= kOffsetSize ? -1 : window_height - kOffsetSize}))
        {
            ImGui::TextUnformatted(raw_output_.c_str());
            if (auto_scroll_)
                ImGui::SetScrollHereY(1.0f);
        }
        ImGui::EndChild();
    }

    void RawTextOutput::addData(std::span<uint8_t> data)
    {
        raw_output_.append(data.begin(), data.end());
    }
} // namespace gui
