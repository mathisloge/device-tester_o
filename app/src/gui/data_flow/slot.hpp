#pragma once
#include <string>
#include <atomic>
namespace gui::df
{
    class BaseSlot
    {
    public:
        explicit BaseSlot(const std::string &name);
        void id(const int id);
        int id() const;
        virtual void operator()();
        virtual void draw();

    private:
        const std::string name_;
        int id_;
    };
} // namespace gui::df
