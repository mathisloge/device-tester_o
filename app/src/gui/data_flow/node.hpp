#pragma once
#include <vector>
#include <unordered_set>
#include <unordered_map>
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
        using InSlots = std::vector<InputSlot>;
        using OutSlots = std::vector<OutputSlot>;
        using InSlotsContainer = std::unordered_map<int, InputSlot>;
        using OutSlotsContainer = std::unordered_map<int, OutputSlot>;
        using Connection = std::pair<int, int>;

    public:
        explicit Node(const std::string &title,
                      InSlots &&input_slots,
                      OutSlots &&output_slots);
        int id() const;
        void drawNode();
        const InSlotsContainer &input_slots() const;
        const OutSlotsContainer &output_slots() const;
        virtual ~Node() = default;

    protected:
        virtual void drawCustomContent();

    protected:
        const std::string title_;

    private:
        const int id_;
        bool is_selected_;
        InSlotsContainer input_slots_;
        OutSlotsContainer output_slots_;
        ImVec2 position_;
        static inline std::atomic<int> node_id_counter = 0;
    };
} // namespace gui::df
