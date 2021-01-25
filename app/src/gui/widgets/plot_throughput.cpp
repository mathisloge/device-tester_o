#include <chrono>
#include <numeric>
#include <algorithm>
#include <implot.h>
#include <fmt/format.h>
#include "plot_throughput.hpp"

namespace gui
{
    PlotThroughput::PlotThroughput(size_t window)
        : data_sizes_{window}, avg_data_sizes_{window}, minmax_y_{0, 1}, minmax_x_{0, 1}
    {
    }
    void PlotThroughput::addData(std::span<uint8_t> data)
    {
        const auto now_s = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        const auto new_point_added = data_sizes_.addToHisto(static_cast<DataPoint::DataType>(now_s), static_cast<DataPoint::DataType>(data.size()));
        if (new_point_added)
        {
            const auto sum = std::accumulate(data_sizes_.data.begin(), data_sizes_.data.end(), 0, [](const int a, const DataPoint &b) { return a + b.y; });
            avg_data_sizes_.addPoint(now_s, sum / data_sizes_.data.size());
        }
        const auto [min_y, max_y] = std::minmax_element(data_sizes_.data.begin(), data_sizes_.data.end(), [](const auto &a, const auto &b) { return a.y < b.y; });
        minmax_y_.first = 0;
        minmax_y_.second = max_y->y + 100;
        const auto [min_x, max_x] = std::minmax_element(data_sizes_.data.begin(), data_sizes_.data.end(), [](const auto &a, const auto &b) { return a.x < b.x; });
        minmax_x_.first = min_x->x;
        minmax_x_.second = max_x->x;
    }

    void PlotThroughput::drawPlot()
    {

        ImPlot::SetNextPlotLimits(minmax_x_.first, minmax_x_.second, minmax_y_.first, minmax_y_.second, ImGuiCond_Always);
        if (ImPlot::BeginPlot("##pltthroughput", "time", "bytes", ImVec2(-1, -1), 0, ImPlotAxisFlags_Time))
        {
            if (data_sizes_.data.size() > 0)
                ImPlot::PlotStairs("throughput", &data_sizes_.data[0].x, &data_sizes_.data[0].y, data_sizes_.data.size(), data_sizes_.offset, 2 * sizeof(DataPoint::DataType));
            if (avg_data_sizes_.data.size() > 0)
                ImPlot::PlotLine("avg. throughput", &avg_data_sizes_.data[0].x, &avg_data_sizes_.data[0].y, avg_data_sizes_.data.size(), avg_data_sizes_.offset, 2 * sizeof(DataPoint::DataType));
            ImPlot::EndPlot();
        }
    }
}; // namespace gui
