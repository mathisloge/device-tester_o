#pragma once
#include <string>
#include <atomic>
namespace gui::df
{
    class Slot
    {
    public:
        explicit Slot(const std::string &name, const std::string &datatype);
        int id() const;
        bool canConnectWith(const Slot &rhs) const;
        void drawSlot();

    private:
        virtual void begin() = 0;
        virtual void end() = 0;
        virtual void drawContent();

    protected:
        const int id_;
        const std::string name_;
        const std::string datatype_;

    private:
        static inline std::atomic<int> slot_id_counter = 0;
    };

    class InputSlot : public Slot
    {
    public:
        explicit InputSlot(const std::string &name, const std::string &datatype);

    private:
        virtual void begin() override;
        virtual void end() override;
    };

    class OutputSlot : public Slot
    {
    public:
        explicit OutputSlot(const std::string &name, const std::string &datatype);

    private:
        virtual void begin() override;
        virtual void end() override;
    };
} // namespace gui::df
