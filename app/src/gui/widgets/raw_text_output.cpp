#include "raw_text_output.hpp"
#include <imgui.h>
#include "../imgui_commons.hpp"

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
            ImGui::PushFont(gFonts["Monospace"]);
            ImGui::TextUnformatted(raw_output_.c_str());
            if (auto_scroll_)
                ImGui::SetScrollHereY(1.0f);
            ImGui::PopFont();
        }
        ImGui::EndChild();
    }

    void RawTextOutput::addData(std::span<uint8_t> data)
    {
        std::string str{data.begin(), data.end()};
        // remove all control chars
        str.erase(std::remove_if(str.begin(),
                                 str.end(),
                                 [](unsigned char x) { return !std::isprint(x) && !std::isspace(x) && x != 127; /* 127 = del char */ }),
                  str.end());
        raw_output_.append(std::move(str));
    }
} // namespace gui
