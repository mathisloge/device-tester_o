#pragma once
#include <vector>
#include <boost/noncopyable.hpp>
#include <imgui.h>
#include "data_flow_types.hpp"
#include "slot.hpp"
namespace gui::df
{
    class UiNode
    {
    public:
        using Slots = std::vector<BaseSlot>;

    public:
        explicit UiNode(const int vertex_id,
                        const std::string &title,
                        const Slots &inputs,
                        const Slots &outputs);
        virtual void draw(const Graph &g);
        Slots &inputs();
        Slots &outputs();

    private:
        const int vertex_id_;
        const std::string title_;
        Slots inputs_;
        Slots outputs_;
    };
} // namespace gui::df
