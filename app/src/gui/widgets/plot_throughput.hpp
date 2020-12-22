#pragma once
#include <vector>
#include <cstdint>
#include <span>
#include <boost/circular_buffer.hpp>
#include <imgui.h>
#include "../imgui_commons.hpp"
namespace gui
{
    class PlotThroughput
    {
    public:
        explicit PlotThroughput(size_t window = 100);
        void addData(std::span<uint8_t> data);
        void drawPlot();

    private:
        struct DataPoint
        {
            using DataType = uint64_t;
            DataType x;
            DataType y;
        };
        ScrollingBuffer<DataPoint, DataPoint::DataType> data_sizes_;
        ScrollingBuffer<DataPoint, DataPoint::DataType> avg_data_sizes_;
        std::pair<DataPoint::DataType, DataPoint::DataType> minmax_y_;
        std::pair<DataPoint::DataType, DataPoint::DataType> minmax_x_;
    };
} // namespace gui
