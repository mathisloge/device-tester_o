#pragma once
#include <vector>
#include <unordered_set>
#include <memory>
#include <atomic>
#include <boost/noncopyable.hpp>
#include <imgui.h>
#include "slot.hpp"
namespace gui::df
{

   
    class Node : public boost::noncopyable
    {
    public:
        using Slots = std::vector<Slot>;
        using Connection = std::pair<int, int>;

    public:
        explicit Node(const std::string &title,
                      const Slots &input_slots,
                      const Slots &output_slots);
        void drawNode();
        virtual ~Node() = default;

    protected:
        virtual void drawCustomContent();

    protected:
        const std::string title_;

    private:
        const int id_;
        bool is_selected_;
        Slots input_slots_;
        Slots output_slots_;
        ImVec2 position_;
        static inline std::atomic<int> node_id_counter = 0;
    };
} // namespace gui::df
