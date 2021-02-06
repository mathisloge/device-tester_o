#pragma once
#include <string>
#include <atomic>
namespace gui::df
{
    class Slot
    {
    public:
        enum class Type
        {
            input,
            output
        };

    public:
        explicit Slot(const std::string &name, const Type type);
        void drawSlot();

    protected:
        virtual void drawContent();

    private:
        const int id_;
        const std::string name_;
        const Type type_;

        static inline std::atomic<int> slot_id_counter = 0;
    };
} // namespace gui::df
