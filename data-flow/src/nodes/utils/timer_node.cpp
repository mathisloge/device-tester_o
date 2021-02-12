#pragma once
#include "data-flow/nodes/utils/timer_node.hpp"
#include "data-flow/slots/trigger_slot.hpp"
#include "data-flow/slots/int_slot.hpp"
#include <thread>
#include <chrono>
#include <imgui.h>
namespace dt::df
{
    class TimerNode::Impl final
    {
    public:
        explicit Impl(std::shared_ptr<IntSlot> int_slot,
                      std::shared_ptr<TriggerSlot> trigger_slot)
            : int_slot_{int_slot},
              trigger_slot_{trigger_slot},
              timer_freq_{100},
              running_{true},
              timer_thread_{std::bind(&Impl::timerThreadFnc, this)}
        {
            int_slot_->subscribe([this](const BaseSlot *) {
                timer_freq_ = int_slot_->valueInt();
            });
        }
        ~Impl()
        {
            running_ = false;
            if (timer_thread_.joinable())
            {
                timer_thread_.join();
            }
        }

    private:
        void timerThreadFnc()
        {
            while (running_)
            {
                auto now = std::chrono::system_clock::now();
                std::this_thread::sleep_until(now + std::chrono::milliseconds(timer_freq_));
                trigger_slot_->trigger();
            }
        }

    private:
        friend TimerNode;
        const NodeKey key_;
        std::shared_ptr<IntSlot> int_slot_;
        std::shared_ptr<TriggerSlot> trigger_slot_;
        int timer_freq_;
        bool running_;

        std::thread timer_thread_;
    };

    TimerNode::TimerNode(const NodeId id,
                         const SlotId input_timer_id,
                         const SlotId output_trigger_id)
        : BaseNode{id, "Timer", kNodeKey,
                   Slots{std::make_shared<IntSlot>(input_timer_id, SlotType::input, "milliseconds")},
                   Slots{std::make_shared<TriggerSlot>(output_trigger_id, SlotType::output, "trigger", SlotFieldVisibility::never)}},
          impl_{new TimerNode::Impl{
              std::dynamic_pointer_cast<IntSlot>(inputs()[0]),
              std::dynamic_pointer_cast<TriggerSlot>(outputs()[0])}}
    {
    }

    TimerNode::~TimerNode()
    {
        delete impl_;
    }
} // namespace dt::df
