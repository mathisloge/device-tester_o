#pragma once
#include <vector>
#include <unordered_set>
#include <memory>
#include <boost/noncopyable.hpp>
#include <imgui.h>
#include <ImNodes.h>
#include <ImNodesEz.h>
#include "connection.hpp"
namespace gui::df
{
    class Node : public boost::noncopyable
    {
    public:
        using Slots = std::vector<ImNodes::Ez::SlotInfo>;

    public:
        explicit Node(const std::string &title,
                      const Slots &input_slots,
                      const Slots &output_slots);
        void deleteConnection(const Connection &connection);
        void drawNode();
        virtual ~Node() = default;

    protected:
        virtual void drawCustomContent();

    protected:
        const std::string title_;

    private:
        bool is_selected_;
        Slots input_slots_;
        Slots output_slots_;
        std::vector<Connection> connections_;
        ImVec2 position_;
    };
} // namespace gui::df
