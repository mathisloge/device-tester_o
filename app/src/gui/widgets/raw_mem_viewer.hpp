#pragma once
#include <cstdint>
#include <span>
#include <vector>
#include <imgui.h>
#include <imgui_memory_editor.h>
namespace gui
{
    class RawMemViewer
    {
    public:
        RawMemViewer();
        void addData(std::span<uint8_t> data);
        void draw();

    private:
        MemoryEditor mem_edit_;
        bool auto_scroll_;
        std::vector<uint8_t> data_;
    };
} // namespace gui
