#pragma once
#include <cstdint>
#include <span>
#include <string>
namespace gui
{
    class RawTextOutput
    {
    public:
        RawTextOutput();
        void addData(std::span<uint8_t> data);
        void draw();

    private:
        std::string raw_output_;
        bool auto_scroll_;
    };
} // namespace gui
